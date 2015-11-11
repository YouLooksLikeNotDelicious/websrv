(function($) {

    var g_slider = new Slider('.z-slide-content', '.z-slide-item');
    var search = parseSearchUrl(location.search);
    var raids;

    if (search && search.action) {
        setItems(search);
        localStorage.setItem('indexLink', JSON.stringify(search));
        localStorage.setItem('cur_track', g_track);
    } else {
        var track = localStorage.getItem('cur_track');
        if(track == g_track) {
            var tmp = localStorage.getItem('indexLink');
            search = JSON.parse(tmp);
            if(search && search.action) {
                setItems(search);
            }
        } else {
            localStorage.removeItem('indexLink');
        }
    }

    // slide event
    document.getElementsByClassName('z-slide-content')[0].addEventListener('slideend', function(e) { 
        var curItem = e.currentItem;
        //var type = curItem.getAttribute('data-type');
        var id = curItem.getAttribute('data-id');
        (new Image()).src = '/stat/show_recommend_content:' + id;
    }, false);

    // recommend_ad listPage
    raids = Array.prototype.slice.call(document.querySelectorAll('section.flat-container[data-uuid]')).map(function(node) {
        return node.getAttribute('data-uuid');
    }) || [];
    (new Image()).src = '/recommend_ad/listPage?raids=' + raids.join(',');

    function setItems(search) {
        var action = search.action;
        var para = $('.attension-zone p')[0];
        if(action === 'clickWifiLogin') {
            para.textContent = '登录成功！您已可以正常使用网络！';
        } else {
            if(search.urlVisitedBefore && search.urlVisitedBefore.length) {
                para.innerHTML = "<a href='" + decodeURIComponent(search.urlVisitedBefore) + "'>登录成功！点击回到您之前访问的网页>></a>";
            } else {
                para.textContent = '登录成功！您已可以正常使用网络！';
            }
        }

        if (search.isLoggedIn) { //登录后显示
            removeClass($('.attension-zone')[0], 'hidden');
        } else { //未登录
            removeClass($('.action-zone')[0], 'hidden');
            $('.internet-access')[0].setAttribute('href', decodeURIComponent(search.backUrl));
        }
    }

})($);