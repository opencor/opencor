$(window).on('load', function() {
    jQuery(document).ready(function($) {
        // Create a div for our popup image

        $("body").append("<div class=\"imagepopup\"><img class=\"popup\"></div>");

        // Disable the mousewheel when the div for our popup image is visible

        $("div.imagepopup").bind("mousewheel", function() {
            return false;
        });

        // Initially hide the div for our popup image

        hideImagePopup();

        // Show and update the contents of the div for our popup image

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

        // Hide the div for our popup image when clicking on it

        $("div.imagepopup").click(function() {
            hideImagePopup();
        });

        // Ignore all key strokes, except the ESC key, when the div for our
        // popup image is visible

        $(document).keydown(function(event) {
            if ($("div.imagepopup").css("visibility") === "visible") {
                if (event.keyCode !== 27)
                    event.preventDefault();
            }
        });

        // Hide the div for our popup image if the ESC key is pressed when our
        // div is visible

        $(document).keyup(function(event) {
            if (   ($("div.imagepopup").css("visibility") === "visible")
                && (event.keyCode === 27)) {
                    hideImagePopup();
            }
        });

        // Hide the div for our popup image

        function hideImagePopup() {
            $("div.imagepopup").css("visibility", "hidden");
        }
    });
});
