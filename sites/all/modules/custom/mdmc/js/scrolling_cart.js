(function ($) {

Drupal.behaviors.scrollingCart = {
  attach: function (context, settings) {
    var offsetPixels = 60;
    var cartTop = $('#block-commerce-cart-cart').offset().top;
    $(window).scroll(function() {
      if ($(window).scrollTop() > cartTop) {
        $( "#block-commerce-cart-cart" ).css({
          "position": "fixed",
          "top": "15px",
          "margin-top": "0"
        });
      } else {
        $( "#block-commerce-cart-cart" ).css({
          "position": "relative",
          "margin-top": "60px"
        });
      }
    });
  }
};

})(jQuery);


