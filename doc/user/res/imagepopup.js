$(window).load(function() {
    jQuery(document).ready(function($) {
        $("body").append("<div class=\"imagepopup\"><img class=\"popup\"></div>");

        hideImagePopup();

        $("[imagepopup]").click(function() {
            $("div.imagepopup").css("visibility", "visible");

            var imagepopup = $("div.imagepopup img.popup");

            $(imagepopup).attr("src", $(this).attr("src"));

            $(imagepopup).css({
                "width": $(imagepopup).width(),
                "height": $(imagepopup).height(),
                "margin-top": -0.5*$(imagepopup).innerHeight(),
                "margin-left": -0.5*($(imagepopup).innerWidth())
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
