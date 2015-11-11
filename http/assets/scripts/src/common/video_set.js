;(function() {
    var group = document.getElementById('sery-group-container');
    var listContainer = document.getElementsByClassName('sery-list')[0];
    var allItems = document.querySelectorAll('.sery-list .list-item');
    var tpl = document.getElementById('seryGroup').innerHTML;
    var htmlGen = Template.parse(tpl);

    var playBox = document.querySelector('.detail-poster');
    var downBtn = document.querySelector('.btn-download');
    var episodeTag = document.getElementById('episode-index');
    // it's static node list, not live!!!
    // var allEpisodes = document.querySelectorAll('.video-item');
    var matches = function(el, selector) {
        if(el.matches) {
            return el.matches(selector);
        } else if(el.webkitMatchesSelector){
            return el.webkitMatchesSelector(selector);
        } else {
            var all = el.parentNode.querySelectorAll(selector);
            var len = all.length;
            while(len--) {
                if(all[len] === el) {
                    return true;
                }
            }
            return false;
        }
    };
    var selected = 1;

    listContainer.addEventListener('click', function(e) {
        var target = e.target;
        var valid = false;
        var text;
        var re;
        var className;
        var i;
        var len;
        if (matches(target, '.sery-ranger')) {
            valid = true;
            target = target.parentNode;
        } else if (matches(target, '.triangle')) {
            valid = true;
            target = target.parentNode.parentNode;
        } 
        if(!valid) {
            return;
        }
        re = /active/;
        if(re.test(target.className)) {
            removeClass(target, 'active');
            group.style.display = 'none';
        } else {
            text = target.textContent.trim();
            request({
                url: '/video_set/detail?uuid=' + group.getAttribute('data-uuid') + 
                    '&count=20&start=' + (+text.split('-')[0] - 1),
                success: function(res) {
                    res.data.selected = selected;
                    group.innerHTML = htmlGen(res.data);
                    group.style.display = 'block';
                }
            });
            for(i = 0, len = allItems.length; i < len; i++) {
                removeClass(allItems[i], 'active');
            }
            addClass(target, 'active');
        }
    }, false);

    group.addEventListener('click', function(e) {
        var target = e.target;
        var allEpisodes;
        if(!matches(target, '.video-item')) {
            return;
        }
        if(/(^|\s+)active($|\s+)/.test(target.className)) {
            return;
        }
        allEpisodes = document.querySelectorAll('.video-item');
        for(var i = 0, len = allEpisodes.length; i < len; i++) {
            removeClass(allEpisodes[i], 'active');
        }
        addClass(target, 'active');
        playBox.setAttribute('data-url', target.getAttribute('data-file-path'));
        downBtn.setAttribute('data-id', target.getAttribute('data-id'));
        selected = target.textContent.trim();
        episodeTag.textContent = selected;
    }, false);
})();
