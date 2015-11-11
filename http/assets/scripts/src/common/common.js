var getForm = function() {
    var form = document.querySelector('form'),
        url = form.action,
        method = form.method;

    return {
        'url': url,
        'method': method
    };
};

var getFields = function() {
    var form = document.querySelector('form'),
        values = {};
    var fields = form.querySelectorAll('input,select,textarea');
    for (var i = 0; i < fields.length; i++) {
        var field = fields[i];
        if (field.type == 'checkbox') {
            if (field.checked) {
                values[field.name] = field.value;
            }
        } else {
            values[field.name] = field.value;
        }
    }
    return values;
};

var serializeKeyVals = function(values) {
    var data = [];
    for (var name in values) {
        data.push(name + '=' + values[name]);
    }
    return data.join('&');
};

var addClass = function(el, classStr) {
    if (!el || !classStr) return;
    classStr = classStr.trim();
    var list;
    var className;
    if (el.classList) {
        list = classStr.split(' ');
        for (var i = 0, len = list.length; i < len; i++) {
            el.classList.add(list[i]);
        }
    } else {
        className = el.className;
        if(!className) {
            el.className = classStr;
            return el;
        }
        list = classStr.split(' ');
        for (var i = 0, len = list.length; i < len; i++) {
            if(!new RegExp('(\\s+|^)' + list[i] + '(\\s+|$)').test(className)) {
                className += ' ' + list[i];
            }
        }
        el.className = className;
    }
    return el;
};

var removeClass = function(el, classStr) {
    if (!el) {
        return;
    }
    if(classStr == null) {
        el.className = '';
        return el;
    }
    var list = classStr.trim().split(' ');
    var className;
    if (el.classList) {
        for (var i = 0, len = list.length; i < len; i++) {
            el.classList.remove(list[i]);
        }
    } else {
        className = el.className;
        for (var i = 0, len = list.length; i < len; i++) {
            className = className.replace(new RegExp('(\\s+|^)' + list[i] + '(\\s+|$)'), ' ');
        }
        el.className = className;
    }
    return el;
};

/**
 * 发起ajax请求
 * @param  {object} options 配置项
 *    @param {string} url
 *    @param {string} data
 *    @param {string} method GET/POST
 *    @param {Function} success
 *    @param {Function} error
 *    @param {Function} complete
 * @return {[type]}         [description]
 */
var request = function(options) {
    var xhr = new window.XMLHttpRequest();
    //todo
    options.method = 'GET';

    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4) {
            xhr.onreadystatechange = function() {};
            var result, error = false;
            if ((xhr.status >= 200 && xhr.status < 300) || xhr.status == 304) {
                dataType = 'json';
                result = xhr.responseText;

                try {
                    // http://perfectionkills.com/global-eval-what-are-the-options/
                    result = /^\s*$/.test(result) ? null : JSON.parse(result);
                    if (result && result.status == 438) {
                        if (options.url.indexOf('test_login_for_download') != -1) {
                            if (confirm("You need to sign in before download.")) {
                                locate(ROOT + '/Account/loginPage');
                            }
                        } else {
                            if (confirm("Session timeout or you've never sign in. \n Sign in now?")) {
                                locate(ROOT + '/Account/loginPage');
                            }
                        }
                        return;
                    }
                } catch (e) {
                    if (options.url.indexOf('test_login_for_download') == -1) {
                        error = e;
                    }
                }

                if (error && options.error) {
                    options.error('parse error', 'parsererror', xhr, options);
                } else {
                    options.success(result, xhr, options);
                }
            } else {
                if (options.error) {
                    options.error(xhr.statusText || null, xhr.status ? 'error' : 'abort', xhr, options);
                }
            }
            if (options.complete) {
                options.complete(xhr);
            }
        }
    };

    var method = options.method || 'GET';
    if (options.url.indexOf('?') == -1) {
        options.url += '?';
    } else {
        options.url += '&';
    }
    options.url += 'isAjax=true';
    if (method.toUpperCase() == 'GET' && options.data) {
        if (options.url.indexOf('?') == -1) {
            options.url += '?';
        } else {
            options.url += '&';
        }
        options.url += options.data;
    }
    xhr.open(method, options.url, true);
    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhr.setRequestHeader('X-Requested-With', 'XMLHttpRequest');
    xhr.setRequestHeader('HTTP_X_REQUESTED_WITH', 'XMLHttpRequest');

    xhr.send(options.data ? options.data : null);
    return xhr;
};

function isElement(o) {
    return (
        typeof HTMLElement === "object" ? o instanceof HTMLElement : //DOM2
        o && typeof o === "object" && o !== null && o.nodeType === 1 && typeof o.nodeName === "string"
    );
}

var $ = function(selector) {
    $.data = $.data || {};
    var el;
    if (isElement(selector)) {
        el = selector;
    } else {
        el = document.querySelector(selector);
    }
    var timestamp = el.getAttribute('data-timestamp');
    if (!timestamp) {
        timestamp = +(new Date());
        el.setAttribute('data-timestamp', timestamp);
    }
    $.data[timestamp] = $.data[timestamp] || {};
    $this = {
        button: function(status) {
            var attr = 'data-' + status + '-text',
                html = el.getAttribute(attr) || status;

            if (status != 'reset') {
                $this.data('html', el.innerHTML);
                $this.data('className', el.className);

                el.innerHTML = html;
                el.disabled = true;
                el.className += ' disabled';
            } else {
                el.innerHTML = $this.data('html');
                el.className = $this.data('className');
                el.disabled = false;
            }
        },
        data: function(key, val) {
            if (val === undefined) {
                return $.data[timestamp][key];
            } else {
                $.data[timestamp][key] = val;
                return this;
            }
            if (key === undefined) {
                return $data[timestamp];
            }
        }
    };
    $this[0] = el;

    return $this;
};

$.trim = function(str) {
    return str === null ? "" : String.prototype.trim.call(str);
};

$.ajax = request;

/*parse url*/
var parseSearchUrl = function(url) {
    if (!/\?\S+/.test(url)) {
        return null;
    }
    url = '&' + url.substring(1);
    var parseReg = /(?:&)([^=]+)=([^&]+)/g;
    var map = {};
    var keyValue;
    while ((keyValue = parseReg.exec(url))) {
        var val = decodeURIComponent(keyValue[2]);
        val === "true" && (val = true);
        val === "false" && (val = false);
        map[keyValue[1]] = val;
    }
    return map;
}

var buy = function(btn, e, type) {
    e = window.event || e;
    e.preventDefault();
    e.stopPropagation();
    //test login
    $.ajax({
        url: ROOT + '/Account/accountPage?test_login_for_download=1',
        success: function() {
            doBuy(btn, e, type);
        }
    });
    //if (g_isLogin === 0) {
    //if (confirm('You need to sign in before download.')) {
    //locate(ROOT + '/Account/loginPage?from=' + encodeURIComponent(location.href));
    //}
    //return;
    //}
};
var doBuy = function(btn, e, type) {
    if (!confirm('Are you sure to buy this item?')) {
        return;
    }
    var uuid = btn.getAttribute('data-uuid'),
        buyUrl = ROOT + '/Content/buy',
        $btn = $(btn);

    $btn.button('buying');
    $.ajax({
        url: buyUrl,
        data: 'uuid=' + uuid,
        method: 'POST',
        success: function(response) {
            $btn.button('reset');
            if (response && response.status == 1000) {
                if (type != 'picture') {
                    btn.onclick = function() {
                        download(btn, e, type);
                    };
                    btn.innerHTML = 'Download';
                    btn.className = btn.className.replace('btn-primary', 'btn-success');
                    download(btn, e, type);
                } else {
                    btn.onclick = function() {
                        locate(ROOT + '/Content/detailPage?uuid=' + uuid);
                        //location = ROOT + '/Content/detailPage?uuid=' + uuid;
                    };
                    btn.innerHTML = 'View';
                    btn.className = btn.className.replace('btn-primary', 'btn-success');
                }
            } else if (response && response.status == 20) {
                alert(response.msg || response.errMsg || 'No much money, please recharge.');
                locate(ROOT + '/Account/rechargePage');
                //location = ROOT + '/Account/rechargePage';
            } else {
                alert(response.msg || response.errMsg || 'Buy failed.');
            }
        },
        error: function(txtStatus, errMsg) {
            var msg = 'Buy failed. ' + txtStatus + ': ' + errMsg;
            alert(msg);
            $btn.button('reset');
        }
    });
};

// recoId is recommend id, only used in index page
var download = function(btn, e, type, topic, recoId) {
    e = window.event || e;
    e.preventDefault();
    e.stopPropagation();
    if (type == 'picture_detail') {
        location.reload();
    } else {
        var search = location.search && parseSearchUrl(location.search) || {};
        var from = search.from;
        if(!from) {
            from = '';
        } /*else if(search.keepFrom) {
            from = search.from;
        }*/
        if(recoId !== undefined) {
            from = 'recommend_ad:' + recoId;
        }
        if(search.topic && !topic) {
            topic = search.topic;
        }
        var uuid = btn.getAttribute('data-uuid');
        var downloadUrl = '/download?uuid=' + uuid + 
            '&url=' + encodeURIComponent(location.pathname) + 
            '&timestamp=' + Date.now() + 
            '&from=' + encodeURIComponent(from);
        var id;
        if(type === 'multi-video' && (id = btn.getAttribute('data-id'))) {
            downloadUrl += '&id=' + id;
        }
        if(topic) {
            downloadUrl += '&topic=' + topic;
        }

        setTimeout(function() {
            location = downloadUrl;
        }, 300);
    }
};
var historyBack = function() {
    var canReplaceHistory = testHistoryReplace();
    if (canReplaceHistory) {
        history.back();
    } else {
        var historyRecord = sessionStorage.getItem(historyKey());
        if (historyRecord) {
            historyRecord = historyRecord.split('||');
            var lastUrl = historyRecord.pop();
            historyRecord = historyRecord.join('||');
            sessionStorage.setItem(historyKey(), historyRecord);
            if (lastUrl) {
                location.replace(lastUrl);
            }
            return;
        }
        location.replace(ROOT + '/Main/index');
    }
};

// type/id will be used to collect data
var locate = function(url, pushState, type, id, additionalTrackId) {
    if (url.indexOf('?') != -1) {
        url += '&track=' + g_track;
    } else {
        url += '?track=' + g_track;
    }

    if(additionalTrackId === undefined) {
        additionalTrackId = 0;
    }
    var search = location.search && parseSearchUrl(location.search) || {};
    var from, topic;
    
    if(type) {
        // 点击焦点图 [ad, content, contentset]
        if(/clickslide:/.test(type)) {  
            // url track --- from
            from = 'recommend_content:' + id;
            topic = additionalTrackId;
            // click data
            (new Image()).src = '/stat/click_recommend_content:' + id;
        // 点击主页推荐位 -- >专题/内容详情页
        } else if(/clickRecommendAd:/.test(type)) {
            from = 'recommend_ad:' + id;
            topic = additionalTrackId;
        // 点击主页推荐位 -- >内容详情页
        }
    }
    if(search.keepFrom) {
        from = search.from;
    }
    if(!from) {
        from = encodeURIComponent(location.pathname);
    }
    url += '&from=' + from;
    if(topic) {
        url += '&topic=' + topic;
    }
    var canReplaceHistory = testHistoryReplace();
    setTimeout(function() {
        if (canReplaceHistory) {
            if (pushState === undefined || pushState === true) {
                location = url;
            } else {
                location.replace(url);
            }
        } else {
            if (pushState === undefined || pushState === true) {
                var historyRecord = sessionStorage.getItem(historyKey()) || '';
                historyRecord += '||' + location.href;
                sessionStorage.setItem(historyKey(), historyRecord);
            } else if (pushState === 'clear') {
                sessionStorage.setItem(historyKey(), '');
            }
            location.replace(url);
        }
    }, 100);
};
var historyKey = function() {
    return 'history_' + g_track;
};
var testHistoryReplace = function() {
    var agentStr = navigator.userAgent;
    // 如果不是360, 认为可以ReplaceHistory
    if (agentStr.indexOf('360browser') == -1) {
        return true;
    }
    return false;
};
var listenMusic = function(e, btn) {
    var audios = document.querySelectorAll('audio');
    for (var i = 0; i < audios.length; i++) {
        audios[i].parentNode.removeChild(audios[i]);
    }

    e = window.event || e;
    e.preventDefault();
    e.stopPropagation();
    //歌曲名称
    var name = btn.getAttribute('data-name');
    var musicName = name || btn.parentElement.parentElement.querySelector('.content-title').innerHTML;
    document.querySelector('#musicName').innerHTML = musicName;
    document.getElementById('audioDiv').className = 'centered-modal-ct open';
    var url = btn.getAttribute('data-url');
    var audioEl = document.createElement('audio');
    document.getElementById('audio').appendChild(audioEl);
    audioEl.controls = true;
    audioEl.src = url;
    audioEl.play();

    return false;
};
var stopListen = function(e) {
    e = window.event || e;
    var audioEl = document.querySelector('#player audio');
    audioEl.pause();
    document.getElementById('audioDiv').className = 'centered-modal-ct';

    e.preventDefault();
    e.stopPropagation();
};
var playPreview = function(e, btn) {
    e = window.event || e;
    e.preventDefault();
    e.stopPropagation();

    var url = btn.getAttribute('data-url');
    var search = location.search && parseSearchUrl(location.search) || {};
    var from = search.from;
    var topic = search.topic;

    url += /\?/.test(url) ? '&' : '?';
    url += 'url=' + encodeURIComponent(location.pathname) + 
        '&timestamp=' + Date.now();
    if(from) {
        url += '&from=' + from;
    }
    if(topic) {
        url += '&topic=' + topic;
    }

    location = url;
};
