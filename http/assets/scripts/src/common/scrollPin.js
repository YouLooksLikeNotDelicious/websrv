;(function() {

    function ScrollPin(el, opts) {
        var self = this,
            options = opts || {};
        self.fixClass = options.fixClass || 'fixed-to-top';
        self.afterScroll = options.afterScroll;
        self.el = el;
        self.top = el.getBoundingClientRect().top + window.pageYOffset;

        var scroll = throttle(function() {
            if(self.top <= window.pageYOffset) {
                self.el.classList.add(self.fixClass);
                if(typeof self.afterScroll === 'function') {
                    self.afterScroll('success', self);
                }
            } else {
                self.el.classList.remove(self.fixClass);
                if(typeof self.afterScroll === 'function') {
                    self.afterScroll('fail', self);
                }
            }
        }, 150);
        window.addEventListener('scroll', scroll);
    };

    function getScrollTop() {
        return window.pageYOffset
    }

    // 函数节流， 规定时间内只执行一次
    function throttle(func, wait) {
        var context, args, result;
        var timeout = null;
        var previous = 0;
        var later = function() {
            previous = current();
            timeout = null;
            result = func.apply(context, args);
            if (!timeout) context = args = null;
        };
        return function() {
            var now = current();
            if (!previous) previous = now;
            var remaining = wait - (now - previous);
            context = this;
            args = arguments;
            if (remaining <= 0 || remaining > wait) {
                clearTimeout(timeout);
                timeout = null;
                previous = now;
                result = func.apply(context, args);
                if (!timeout) context = args = null;
            } else if (!timeout) {
                timeout = setTimeout(later, remaining);
            }
            return result;
        };
    };

    function current() {
        return Date.now() || new Date().getTime();
    };

    // export 
    window.ScrollPin = ScrollPin;

})();