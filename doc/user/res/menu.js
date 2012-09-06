function showContentsMenu(relativePath) {
    initContentsMenu(relativePath, 54, 96, 146);

    document.write("<ul class=\"contentsMenu\">");
    document.write("    <li>");
    document.write("        <img src=\""+relativePath+"/../res/pics/oxygen/actions/help-about.png\" width=24 height=24 alt=\"Contents\">");
    document.write("        <ul>");
    document.write("            <li><a href=\""+relativePath+"/index.html\">Home</a></li>");
    document.write("            <li><a href=\""+relativePath+"/dissemination.html\">Dissemination</a></li>");
    document.write("            <li><a href=\""+relativePath+"/knownIssuesLimitations.html\">Known issues/limitations</a></li>");
    document.write("            <li><a href=\""+relativePath+"/licensing.html\">Licensing</a></li>");
    document.write("            <li><a href=\""+relativePath+"/authors.html\">Authors</a></li>");
    document.write("            <li class=\"lastContentsMenuItem\"><a href=\""+relativePath+"/contactUs.html\">Contact us</a></li>");
    document.write("        </ul>");
    document.write("    </li>");
    document.write("</ul>");
}

$(document).ready(function() {
    $("ul.contentsMenu > li").mouseenter(function() {
        $(this).find('ul').css('visibility', 'visible');
    });

    $("ul.contentsMenu > li").mouseleave(function() {
        $(this).find('ul').css('visibility', 'hidden');
    });
});
