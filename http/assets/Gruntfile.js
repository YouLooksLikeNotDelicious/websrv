module.exports = function (grunt) {
    var getImgSize = function (imgsrc) {
        var path = require('path');
        var imgpath = path.join(imgsrc.currentFileInfo.currentDirectory, imgsrc.value);
        var sizeOf = require('image-size');
        var dimensions = sizeOf(imgpath);
        return dimensions;
    };
    // Project configuration.
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        uglify: {
            dist: {
                files: {
                    './dist/scripts/src/common/common.js': ['./dist/scripts/src/common/common.js'],
                    './dist/scripts/src/common/sliders.js': ['./dist/scripts/src/common/sliders.js']
                }
            }
        },
        less: {
            source: {
                options: {
                    paths: ['./content/less/Shared/', './content/less/Common/'],
                    compress: false,
                    relativeUrls: true,
                    sourceMap: true,
                    customFunctions: {
                        'image-size': function (less, imgsrc) {
                            var size = getImgSize(imgsrc);
                            return size.width + 'px ' + size.height + 'px;';
                        },
                        'image-width': function (less, imgsrc) {
                            var size = getImgSize(imgsrc);
                            return size.width + 'px';
                        },
                        'image-height': function (less, imgsrc) {
                            var size = getImgSize(imgsrc);
                            return size.height + 'px';
                        },
                        'height-resolute-url': function (less, imgsrc) {
                            var path = require('path');
                            imgsrc = imgsrc.value;
                            var ext = path.extname(imgsrc);
                            console.log(path.dirname(imgsrc) + '/' + path.basename(imgsrc, ext) + '@2x' + ext);
                            return path.dirname(imgsrc) + '/' + path.basename(imgsrc, ext) + '@2x' + ext;
                        }
                    }
                },
                files: {
                    './content/css/Main/index.css': './content/less/Main/index.less',
                    './content/css/Search/index.css': './content/less/Search/index.less',
                    './content/css/Search/result.css': './content/less/Search/result.less',
                    './content/css/Search/detailResult.css': './content/less/Search/detailResult.less',
                    './content/css/Search/detailResultWallpaper.css': './content/less/Search/detailResultWallpaper.less',
                    './content/css/Account/form.css': './content/less/Account/form.less',
                    './content/css/Account/account.css': './content/less/Account/account.less',
                    './content/css/Content/wallpaper.css': './content/less/Content/wallpaper.less',
                    './content/css/Content/video.css': './content/less/Content/video.less',
                    './content/css/Content/content.css': './content/less/Content/content.less',
                    './content/css/Content/detail.css': './content/less/Content/detail.less',
                    './content/css/Content/video-detail.css': './content/less/Content/video-detail.less',

                    './content/css/ContentSet/video.css': './content/less/ContentSet/video.less',
                    './content/css/ContentSet/content.css': './content/less/ContentSet/content.less'
                }
            }
        },
        jshint: {
            source: {
                src: [
                  'Gruntfile.js',
                  'scripts/src/**/*.js',
                  'scripts/specs/*.js'
                ]
            }
        },
        csslint: {
            options: {
                csslintrc: './content/less/csslintrc'
            },
            source: {
                src: ['./content/css/**/*.css']
            }
        },
        lesslint: {
            options: {
                csslintrc: './content/less/csslintrc'
            },
            source: {
                src: [
                    './content/less/Account/*.less',
                    './content/less/Content/*.less',
                    './content/less/Main/*.less',
                    './content/less/Search/*.less',
                ]
            }
        },
        clean: {
            dist: ['dist/', 'release.zip'],
            garbage: ['./dist/dmc/Runtime/'],
            css: ['./content/css/']
        },
        copy: {
            dist: {
                options: {
                    processContent: changeConfig
                },
                src: ['./ThinkPHP/**', './content/css/**', './dmc/**', './scripts/**', 'index.php'],
                dest: 'dist/'
            },
            binary: {
                options: {
                    mode: true
                },
                src: ['./content/img/**', 'logTotal.php', 'process.php'],
                dest: 'dist/'
            },
            zsliderjs: {
                src: ['./bower_components/zSlider/dist/zSlider.js'],
                dest: './scripts/src/common/zSlider.js'
            },
            zsliderless: {
                src: ['./bower_components/zSlider/dist/zSlider.less'],
                dest: './content/less/Common/zSlider.less'
            }
        },
        cssmin: {
            dist: {
                files: {
                    './dist/content/css/Account/account.css': ['./dist/content/css/Account/account.css'],
                    './dist/content/css/Account/form.css': ['./dist/content/css/Account/form.css'],
                    './dist/content/css/Content/content.css': ['./dist/content/css/Content/content.css'],
                    './dist/content/css/Content/detail.css': ['./dist/content/css/Content/detail.css'],
                    './dist/content/css/Content/wallpaper.css': ['./dist/content/css/Content/wallpaper.css'],
                    './dist/content/css/Main/index.css': ['./dist/content/css/Main/index.css'],
                    './dist/content/css/Search/detailResult.css': ['./dist/content/css/Search/detailResult.css'],
                    './dist/content/css/Search/detailResultWallpaper.css': ['./dist/content/css/Search/detailResultWallpaper.css'],
                    './dist/content/css/Search/index.css': ['./dist/content/css/Search/index.css'],
                    './dist/content/css/Search/result.css': ['./dist/content/css/Search/result.css']
                }
            }
        },
        zip: {
            dist: {
                cwd: 'dist',
                src: ["ThinkPHP/**", "content/**", "dmc/**", "scripts/**", "index.php"],
                dest: 'release.zip'
            }
        },
        image_sprite: {
            options: {
                'cssPath': 'content/less/Common',
                'vertical': true,
                'margin': 6
            },
            sprite: {
                options: {
                    'vertical': true,
                    'cssFile': 'icons.less',
                    'retina': function(filepath) {
                        return /large/.test(filepath);
                    }
                },
                files: {
                    'content/img/sprite.png': ['content/img/small/*.png', 'content/img/large/*.png']
                }
            }
        },
        autoprefixer: {
            options: {
                browsers: ['last 2 versions']
            },
            dist: {
                files: [{
                    expand: true,
                    cwd: './content/css/',
                    src: '{,*/}*.css',
                    dest: './content/css/'
                }]
            }
        }
    });

    // Load the plugin that provides the "uglify" task.
    grunt.loadNpmTasks('grunt-contrib-uglify');
    grunt.loadNpmTasks('grunt-contrib-jshint');
    grunt.loadNpmTasks('grunt-contrib-csslint');
    grunt.loadNpmTasks('grunt-lesslint');
    grunt.loadNpmTasks('grunt-contrib-copy');
    grunt.loadNpmTasks('grunt-contrib-clean');
    grunt.loadNpmTasks('grunt-contrib-less');
    grunt.loadNpmTasks('grunt-contrib-cssmin');
    grunt.loadNpmTasks('grunt-zip');
    grunt.loadNpmTasks('grunt-image-sprite');
    grunt.loadNpmTasks('grunt-autoprefixer');
    grunt.registerTask('prepareZSlider', 'prepare zSlider to use', [
        'copy:zsliderjs', 'copy:zsliderless'
    ]);
    grunt.registerTask('genCSS', 'Convert less to css. Of course we should lint them.', [
        'clean:css', 'less:source', 'csslint:source'
    ]);
    grunt.registerTask('validateJS', 'Validate javascript files', [
        'jshint:source'
    ]);
    grunt.registerTask('copySrcAndConfig', 'Copy source code to distribution folder.', [
        'copy:dist', 'copy:binary'
    ]);
    grunt.registerTask('minified', 'Minified css and javascript file in the distribution folder.', [
        'cssmin:dist', 'uglify:dist'
    ]);
    grunt.registerTask('validateOption', 'Validate options passed by command line.', function () {
        var host = grunt.option('host'),
            //apid = grunt.option('apid'),
            adminHost = grunt.option('adminhost'),
            result = true;

        if (!host || !host.trim()) {
            grunt.log.error('--host需要指定, 例如: --host=http://yinni_user.longdata.cc');
            result = false;
        }
        //if (!apid || !apid.trim()) {
            //grunt.log.error('--apid需要指定, 例如: --apid=PENGNIAN_01');
        //  result = false;
        //}
        if (!adminHost || !adminHost.trim()) {
            grunt.log.error('--adminhost需要指定, 例如: --adminhost=http://yinni.longdata.cc');
            result = false;
        }
        return result;
    });
    grunt.registerTask('tar', 'tar gz the distribution dir', function () {
        var done = this.async();
        var process = require('child_process'),
            fs = require('fs');
        process.exec('tar -czf ../wifistore.tar.gz *', {
                cwd: './dist'
            }, function (err) {
            console.log(err);
            done(!err);
        });
    });
    grunt.registerTask('printVersion', 'print the release version', function () {
        var version = grunt.config.get('pkg.version');
        var fs = require('fs'),
            apid = fs.readFileSync('dmc/APID', 'utf8');
        grunt.log.ok('Release verion: ' + version + ' success! The APID is "' + apid + '"');
    });

    grunt.registerTask('release', [
        'validateOption',
        'genCSS',
        'validateJS',
        'clean:dist',
        'copySrcAndConfig',
        'clean:garbage',
        'minified'
    ]);


    function changeConfig(content, srcpath) {
        grunt.log.write('*');
        if (srcpath == 'index.php') {
            return content.replace(/\/dmc\/index\.php\//i, '/dmc/');
        } else if (srcpath == './dmc/index.php') {
            var reg_APP_DEBUG = /'APP_DEBUG',\s*TRUE/i,
                reg_RUNTIME_PATH = /^\/\/\s*(.+?'RUNTIME_PATH',\s*'.+?;)/m;
            var APP_DEBUG = 'FALSE';
            content = content.replace(reg_APP_DEBUG, "'APP_DEBUG', " + APP_DEBUG);
            content = content.replace(reg_RUNTIME_PATH, "$1");
        } else if (srcpath == './dmc/Conf/config.php') {
            var reg_SHOW_PAGE_TRACE = /'SHOW_PAGE_TRACE'\s*=>\s*TRUE,/i,
                reg_APP = /'__APP__'\s*=>\s*'\/dmc\/index\.php/i,
                reg_HOST = /'user_host'\s*=>\s*'.+?'/,
                reg_ADMINHOST = /'admin_host'\s*=>\s*'.+?'/;

            var HOST = grunt.option('host');
                ADMINHOST = grunt.option('adminhost');

            content = content.replace(reg_SHOW_PAGE_TRACE, "'SHOW_PAGE_TRACE' => FALSE,");
            content = content.replace(reg_APP, "'__APP__' => '/dmc");
            content = content.replace(reg_HOST, "'user_host' => '" + HOST + "'");
            content = content.replace(reg_ADMINHOST, "'admin_host' => '" + ADMINHOST + "'");
        } else if (srcpath.indexOf('/Tpl/') != -1) {
            content = addVersion(content, srcpath);
        }
        return content;
    }

    function addVersion(content, srcpath) {
        var version = grunt.config.get('pkg.version');
        var reg_link_css = /<link.+?href\s*=\s*"(.+?\.css)"\s*\/?>/ig,
            reg_srcipt = /<script.+?src\s*=\s*"(.+?\.js)"\s*>/ig;

        content = content.replace(reg_link_css, '<link rel="stylesheet" href="$1?v=' + version + '" />');
        content = content.replace(reg_srcipt, '<script src="$1?v=' + version + '">');
        return content;
    }
};
