!(function() {

    function Viewer(imageSelector, backdropSelector, options) {
        if(!imageSelector) {
            return console.log('Viewer: arguments error.');
        }

        var images, 
            backdrop,
            list, 
            img, 
            count;
        images = typeof imageSelector === 'string' ? document.querySelectorAll(imageSelector) : imageSelector;

        if(!images || !images.length) {
            return warn('Viewer: backdrop or item is empty.');
        }

        if(!backdropSelector) {
            backdrop = document.createElement('div');
            document.body.appendChild(backdrop);
        } else {
            backdrop = typeof backdropSelector === 'string' ? document.querySelector(backdropSelector) : backdropSelector;
        }

        // 合并默认配置
        options = options || {};
        for (var name in defaults) {
            if (options[name] === undefined) {
                options[name] = defaults[name];
            }
        }

        count = images.length;
        this.options = options;
        this.width = window.innerWidth;
        this.height = window.innerHeight;
        this.count = count;
        // calcGoodImageSize = calcImageSize(this.width, this.height);
        list = this.list = []; 
        this.backdrop = backdrop;
        prepare(this, images);
    };

    Viewer.prototype.show = function(img) {
        var self = this,
            index = img.index,
            count = self.count,
            list = self.list,
            backdrop = self.backdrop,
            width = self.width;
        setPosition(list[index], 0);
        for(i = 0; i < index; i++) {
            setPosition(list[i], -width);
        }
        for(i = index + 1; i < count; i++) {
            setPosition(list[i], width);
        }
        backdrop.style.display = 'block';
    };

    Viewer.prototype.hide = function() {
        this.backdrop.style.display = 'none';
    };

    var cloneImage = function(img) {
        var clone = img.cloneNode(),
            div = document.createElement('div');
        div.className = 'z-backdrop-item';
        div.appendChild(clone);
        return div;
    };

    var prepare = function(viewer, images) {
        var backdrop = viewer.backdrop,
            list = viewer.list,
            count = viewer.count,
            width = viewer.width,
            bdClassName = backdrop.className,
            i, item, img;
        backdrop.className = bdClassName + ' z-image-viewer-backdrop';
        for(i = 0; i < count; i++) {
            img = images[i];
            img.index = i;
            bindEventsForImage(img, viewer);

            item = cloneImage(img);
            item.index = i;
            list.push(item);
            backdrop.appendChild(item);
            bindEventsForClonedItem(item, viewer);
        }
        document.addEventListener('touchend', function(ev) {
            if(!eventArgs.action || !eventArgs.curItem) {
                return;
            }
            eventArgs.action = false;
            eventArgs.diffX = ev.changedTouches[0].pageX - eventArgs.startX;
            if(Math.abs(eventArgs.diffX) < 1) {
                viewer.hide();
            }
            if(!eventArgs.next) {
                return;
            }
            setCompatibleStyle(eventArgs.curItem, 'transition', 'transform 0.4s linear');
            setCompatibleStyle(eventArgs.next, 'transition', 'transform 0.4s linear');
            if(eventArgs.diffX < 0) {
                setPosition(eventArgs.curItem, -width);
                setPosition(eventArgs.next, 0);
            } else {
                setPosition(eventArgs.curItem, width);
                setPosition(eventArgs.next, 0);
            } 
        }, false);
    };

    var setPosition = function(item, distanceX) {
        setCompatibleStyle(item, 'transform', 'translate3d(' + distanceX + 'px, 0px, 0px)');
    };

    var bindEventsForImage = function(img, viewer) {
        var action,
            startX;
        img.addEventListener('touchstart', function(ev) {
            action = true;
            startX = ev.touches[0].pageX;
        }, false);
        img.addEventListener('touchend', function(ev) {
            if(action) {
                action = false;
                if(Math.abs(ev.changedTouches[0].pageX - startX) < 7) {
                    viewer.show(img);
                }
            }
        }, false);
    };

    var eventArgs = {
        action: false,
        startX: 0,
        diffX: 0,
        next: null,
        curItem: null
    };

    var bindEventsForClonedItem = function(item, viewer) {
        var list = viewer.list,
            count = viewer.count,
            width = viewer.width,
            index = item.index;
        item.addEventListener('touchstart', function(ev) {
            eventArgs.action = true;
            eventArgs.startX = ev.touches[0].pageX;
            setCompatibleStyle(item, 'transition', '');
            if(index > 0) {
                setCompatibleStyle(list[index - 1], 'transition', '');
            }
            if(index < count - 1) {
                setCompatibleStyle(list[index + 1], 'transition', '');
            }
            eventArgs.curItem = item;
        }, false);
        item.addEventListener('touchmove', function(ev) {
            eventArgs.diffX = ev.changedTouches[0].pageX - eventArgs.startX;
            if(eventArgs.diffX < 0) {
                eventArgs.next = list[index + 1];
                eventArgs.next && setPosition(eventArgs.next, eventArgs.diffX + width);
            } else {
                eventArgs.next = list[index - 1];
                eventArgs.next && setPosition(eventArgs.next, eventArgs.diffX - width);
            }
            eventArgs.next && setPosition(item, eventArgs.diffX);
        }, false);
        /*item.addEventListener('touchend', function(ev) {
            
        }, false);*/
    };

    var upperFirstChar = function(text) {
        var first = text[0],
            left = text.slice(1);
        return first.toUpperCase() + left;
    };
    var setCompatibleStyle = function(el, prop, value) {
        var tmpProp = upperFirstChar(prop),
            moz = 'moz' + tmpProp,
            webkit = 'webkit' + tmpProp;
        if(prop in el.style) {
            el.style[prop] = value;
        } else if(webkit in el.style) {
            el.style[webkit] = value;
        } else if(moz in el.style) {
            el.style[moz] = value;
        } 
    };

    var calcImageSize = function(StageWidth, StageHeight) {
        return function(width, height) {
            var ratio1 = width / height,
                ratio2 = StageWidth / StageHeight,
                w, h;
            if (ratio1 > ratio2) {
                w = StageWidth;
                h = w / ratio1;
            } else {
                h = StageHeight;
                w = ratio1 * h;
            }
            return {
                width: w,
                height: h
            };
        }
    };

    var calcGoodImageSize;

    /**
     * defaults: Viewer默认配置项
    */
    var defaults = Viewer.defaults = {};

    // RequireJS && SeaJS
    if (typeof define === 'function') {
        define(function() {
            return Viewer;
        });
    } else { // default exports to window
        this.Viewer = Viewer;
    }

})();