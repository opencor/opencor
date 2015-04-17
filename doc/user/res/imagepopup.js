$(window).load(function() {
    jQuery(document).ready(function($) {
        $("body").append("<div class=\"imagepopup\"><img class=\"popup\"></div>");

        hideImagePopup();

        $("[imagepopup]").click(function() {
            $("div.imagepopup").css("visibility", "visible");

            var imagepopup = $("div.imagepopup img.popup");

            // Retrieve the size of our image and its padding
            // Note: to use $(imagepopup).width() and $(imagepopup).height() to
            //       retrieve the size of our image is not good enough since
            //       our image won't be loaded yet, so we would effectively be
            //       retrieving the size of our previous image. Now, when it
            //       comes to the padding, it will always be the same for all
            //       our images, hence we can safely use $(imagepopup)...

            var image = new Image();

            image.src = $(this).attr("src");

            var imageWidth = image.width;
            var imageHeight = image.height;
            var imagePaddingX = $(imagepopup).innerWidth()-$(imagepopup).width();
            var imagePaddingY = $(imagepopup).innerHeight()-$(imagepopup).height();

            $(imagepopup).attr("src", $(this).attr("src"));

            $(imagepopup).css({
                "width": imageWidth,
                "height": imageHeight,
                "margin-top": -0.5*(imageHeight+imagePaddingY),
                "margin-left": -0.5*(imageWidth+imagePaddingX)
            });
        });

        $("div.imagepopup").click(function() {
            hideImagePopup();
        });

        $(document).keyup(function(event) {
            if (event.keyCode === 27)
                hideImagePopup();
        });

        function hideImagePopup() {
            $("div.imagepopup").css("visibility", "hidden");
        }
    });
});
