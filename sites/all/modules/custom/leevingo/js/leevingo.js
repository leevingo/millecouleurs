(function ($) {

Drupal.behaviors.toggleExtraInfo = {
  attach: function (context, settings) {
    $('.views-field-field-product-info-slider').hide();
    $('.views-field-field-product-picture-slider').hide();
    $('.views-field-field-product-info-icon').toggle(
      function(){
        $('.views-field-field-product-info-slider').hide();
        $('.views-field-field-product-picture-slider').hide();
        $(this).next().slideDown();
      },
      function(){
        $(this).next().slideUp();
    });
    $('.views-field-field-product-picture-icon').toggle(
      function(){
        $('.views-field-field-product-info-slider').hide();
        $('.views-field-field-product-picture-slider').hide();
        $(this).next().slideDown();
      },
      function(){
        $(this).next().slideUp();
    });
  }
};

})(jQuery);