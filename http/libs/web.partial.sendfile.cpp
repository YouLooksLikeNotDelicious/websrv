#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../libs/web.hpp"
#include "../libs/logger.hpp"
#include "../libs/mimetype.hpp"
#include "../controller/base.hpp"

//---- begin send file ----//
struct download_write_hook_params
{
    zxy_request_context *context;
    std::string file_path;
    off_t file_size;
    off_t pos;
    blksize_t blksize;
    off_t end_pos;
};
static evhtp_res
send_file_write_hook(
        evhtp_connection_t *conn,
        void *arg
        )
{
    download_write_hook_params *params = (download_write_hook_params*)arg;
    zxy_request_context *context = params->context;
    
    int buf_len = params->blksize;
    if ( params->pos == params->end_pos + 1)
    {
        if ( context->request->finished == 0 )
        {
            //log("Finish send file");
            delete params;
            evhtp_unset_hook(&conn->hooks, evhtp_hook_on_write);
            zxy_end_send_file(context);
            return HTTP_OK;
        }
        else
        {
            //zxy_end_request(context, HTTP_OK);
            //evhtp_send_reply_end(context->request);
            return HTTP_OK;
        }
    }
    else if ( params->pos + buf_len > params->end_pos + 1)
    {
        buf_len = params->end_pos - params->pos + 1;
    }

    int fd = open(params->file_path.c_str(), 'r');
    //log("%s: fd %d", params->file_path.c_str(), fd);
    if ( fd == -1 )
    {
        logerr("open file error");
        abort();
    }


    evbuffer *buf = evbuffer_new();
    int err = evbuffer_add_file(buf, fd, params->pos, buf_len);
    params->pos = params->pos + buf_len;
    evhtp_send_reply_body(conn->request, buf);
    evbuffer_free(buf);
    if ( err != 0 )
    {
        logerr("write file error");
        zxy_500(context, "Write file error");
        return HTTP_500;
    }
    return HTTP_OK;
}

void
parse_range_header(
        const std::string& range_header,
        int ranges[]
        )
{
    //range_heade format is: 'bytes=0-' 'bytes=0-10' 'bytes=100-300'
    size_t pos = range_header.find('-');
    std::string start_val = range_header.substr(6, pos - 6 + 1);
    //ranges[0] = std::atoi(start_val.c_str());
    ranges[0] = atoi(start_val.c_str());
    size_t sec_start_pos = pos + 1;
    pos = range_header.find(',', sec_start_pos);
    if ( pos == std::string::npos )
    {
        ranges[1] = -1;
    }
    else
    {
        std::string end_val = range_header.substr(sec_start_pos, pos - sec_start_pos + 1);
        //ranges[1] = std::atoi(end_val.c_str());
        ranges[1] = atoi(end_val.c_str());
    }
}

void
zxy_send_file(
        zxy_request_context *context,
        const std::string& file_path
        )
{
    struct stat f_stat;
    if ( stat(file_path.c_str(), &f_stat) != 0 )
    {
        return zxy_404(context, "Resource not found");
    }
    if ( !S_ISREG(f_stat.st_mode) )
    {
        return zxy_404(context, "Resource not found");
    }
    struct stat file_stat;
    int err = stat(file_path.c_str(), &file_stat);
    if ( err != 0 )
    {
        logerr("Download file failed");
        return zxy_500(context, "Download file failed");
    }

    download_write_hook_params *params = new download_write_hook_params();
    evhtp_set_hook(
            &(context->request->conn->hooks),
            evhtp_hook_on_write,
            (evhtp_hook)send_file_write_hook, 
            params
            );

    off_t file_size = file_stat.st_size;
    params->context = context;
    params->file_path = file_path;
    params->file_size = file_size;
    params->blksize = 8192*3;//file_stat.st_blksize;
    params->end_pos = file_size;
    params->pos = 0;

    std::string mimetype = get_mime_type(file_path.c_str());
    if ( !mimetype.empty() )
    {
        zxy_add_http_header(context, "Content-Type", mimetype);
        zxy_add_http_header(context, "Cache-Control", "max-age=2592000");
    }
    zxy_add_http_header(context, "Accept-Range", "bytes");
    
    std::string range_header = zxy_get_http_header(context, "Range");
    if ( range_header.empty() == false )
    {
        int ranges[2];
        parse_range_header(range_header, ranges);
        int start = ranges[0];
        int end = ranges[1];
        params->pos = start;
        params->end_pos = end == -1 ? file_size - 1 : end;
        char content_range_header[128];
        snprintf(content_range_header, 128, "bytes %ld-%ld/%ld", params->pos, params->end_pos, params->file_size);
        zxy_add_http_header(context, "Content-Length", std::to_string(params->end_pos - params->pos + 1));
        zxy_add_http_header(context, "Content-Range", content_range_header);
        evhtp_send_reply_start(context->request, HTTP_PARTIAL);
    }
    else
    {
        zxy_add_http_header(context, "Content-Length", std::to_string(file_size));
        evhtp_send_reply_start(context->request, HTTP_OK);
    }
}
