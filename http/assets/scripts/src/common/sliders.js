(function () {
  var slidersCtEl = document.querySelector('.sliders'),
    firstSlideEl = document.querySelector('.slide0'),
    x = 0,
    slideCount = slidersCtEl.querySelectorAll('.slide').length,
    currentIndex = 0; //当前显示的图片
  var onSliderMouseDown = function (e) {
    e = window.event || e;
    // e.preventDefault();
    x = e.touches[0].clientX;
    isDown = true;
    var slidersCtEl = document.querySelector('.sliders');
    firstSlideEl.className = 'slide slide0';
    slidersCtEl.addEventListener('touchmove', onSliderMouseMove, true);
    slidersCtEl.addEventListener('touchend', onSliderMouseUp, true);
  };
  var onSliderMouseMove = function (e) {
    e = window.event || e;
    e.preventDefault();
    var x1 = e.changedTouches[0].clientX;
    var bodyWidth = document.body.clientWidth,
      marginLeft = -currentIndex * 100 + (x1 - x) * 100 / bodyWidth;
    firstSlideEl.style.marginLeft = marginLeft + '%';
  };
  var onSliderMouseUp = function (e) {
    e = window.event || e;
    var x1 = e.changedTouches[0].clientX,
      offestX = x1 - x,
      marginLeft;
    if (Math.abs(offestX) > 5) {
      e.preventDefault();
      var slidersCtEl = document.querySelector('.sliders');
      slidersCtEl.removeEventListener('touchmove', onSliderMouseMove, true);
      slidersCtEl.removeEventListener('touchend', onSliderMouseUp, true);
      if (x1 > x && currentIndex === 0) {
        // marginLeft = 0;
      } else if (x1 < x && currentIndex == slideCount - 1) {
        // marginLeft = currentIndex * 100;
      } else if (Math.abs(offestX) < document.body.clientWidth / 4) {
        // marginLeft = currentIndex * 100;
      } else {
        currentIndex = currentIndex + (offestX > 0 ? -1 : 1);
      }
      marginLeft = currentIndex * 100;
      firstSlideEl.className = 'slide slide0 animate-margin';
      firstSlideEl.style.marginLeft = (-marginLeft) + '%';
      var oldActiveEls = slidersCtEl.querySelectorAll('.active');
      for (var i = 0; i < oldActiveEls.length; i++) {
        var el = oldActiveEls[i];
        el.className = el.className.replace('active', '');
      }
      var activeEls = slidersCtEl.querySelectorAll('.slide:nth-child(' + (currentIndex + 1) + '), .indicator li:nth-child(' + (currentIndex + 1) + ')');
      for (i = 0; i < activeEls.length; i++) {
        var activeEl = activeEls[i];
        activeEl.className = activeEl.className + ' active';
      }
    }
  };

  slidersCtEl.addEventListener('touchstart', onSliderMouseDown, true);
})();
