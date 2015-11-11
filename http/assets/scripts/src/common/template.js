'use strict';

;(function umd(global, factory) {

    if(typeof define === 'function' && define.amd) {
        define([], factory);

    } else if(typeof exports === 'object') {
        module.exports = factory();

    } else {
        global.Template = factory();
    }
})(window && window.window === window ? window : this, function factory() {

    var Template = {};

    var noMatch = /(.)^/; // 什么都不匹配

    // 需要转义的字符，转义后才能放入字符串中
    var escapes = {
        "'": "'",
        '\\': '\\',
        '\r': 'r',
        '\n': 'n',
        '\u2028': 'u2028',
        '\u2029': 'u2029'
    };
    var escaper = /\\|'|\r|\n|\u2028|\u2029/g;
    var escapeChar = function(match) {
        return '\\' + escapes[match];
    };

    var cache = Template.cache = {};

    Template.settings = {
        evaluate: /<%([\s\S]+?)%>/g,
        interpolate: /<%=([\s\S]+?)%>/g,
        escape: /<%-([\s\S]+?)%>/g
    };

    Template.parse = function(text, settings, cacheId) {

        if(!text) {
            return;
        }

        cacheId = isEmpty(cacheId) || (cacheId in cache) ? genHashFromStr(text) : cacheId;
        
        // check cacheId
        if(cacheId in cache) {
            return Template.get(cacheId);
        };
        settings = settings ? quickMerge(Template.settings, settings) : Template.settings;

        var matcher = RegExp([
            (settings.escape || noMatch).source, (settings.interpolate || noMatch).source, (settings.evaluate || noMatch).source
        ].join('|') + '|$', 'g');

        var source = "__p+='";
        var index = 0;
        text.replace(matcher, function(match, escape, interpolate, evaluate, offset) {
            source += text.slice(index, offset).replace(escaper, escapeChar);
            index = offset + match.length;

            if (escape) { // 是转义，则把变量的值转义后加到source
                source += "'+\n((__t=(" + escape + "))==null?'':_.escape(__t))+\n'";
            } else if (interpolate) { // 插值，则把变量的值直接加到source
                source += "'+\n((__t=(" + interpolate + "))==null?'':__t)+\n'";
            } else if (evaluate) { // 是js代码，则原样添加到source
                source += "';\n" + evaluate + "\n__p+='";
            }
            return match; // Adobe VMs必须返回match来正确生成offset
        });

        source += "';\n";

        source = "var __t,__p='',__j=Array.prototype.join," +
            "print=function(){__p+=__j.call(arguments,'');};\n" +
            source + 'return __p;\n';

        try { // 构造编译函数，此处为模板引擎核心：
            // 通过构造Function来动态执行js字符串
            var render = new Function(settings.variable || 'data', source);
        } catch (e) {
            e.source = source;
            throw e;
        }

        var template = function(data) { // 编译好的模板函数，调用只需传入数据即可
            return render.call(this, data); // 此处传入unserscore(_)作为参数，所以模板中可以使用unsercore的各种工具函数
        };

        // 把编译好的源码暴露到template.source，外部可以查看
        var argument = settings.variable || 'data';
        template.source = 'function(' + argument + '){\n' + source + '}';

        // cache the result
        Template.cache[cacheId] = template;

        return template;
    };

    Template.get = function(cacheId) {
        return cache[cacheId];
    };


    return Template;


    // utils

    function isEmpty(obj) {
        return obj === undefined || obj === null;
    }

    function quickMerge(defaults, settings) {
        var res = {};
        var prop;
        for (prop in defaults) {
            res[prop] = prop in settings ? settings[prop] : defaults[prop];
        }
        return res;
    }

    // generate hash from string
    // http://stackoverflow.com/questions/7616461/generate-a-hash-from-string-in-javascript-jquery
    function genHashFromStr(str) {
        var hash = 0;
        var i = 0;
        var charCode;
        if (!str.length) {
            return hash;
        }
        for (; i < str.length; i++) {
            charCode = str.charCodeAt(i);
            hash = (hash << 5) - hash + charCode;
            hash |= 0;
        }
        return hash;
    }

});
