function showContentsMenu(relativePath) {
    initContentsMenu(relativePath, 49, 133, 155);

    document.write("<ul class=\"contentsMenu\">");
    document.write("    <li>");
    document.write("        <img src=\""+relativePath+"/../res/pics/oxygen/actions/help-about.png\" width=24 height=24 alt=\"Contents\">");
    document.write("        <ul>");
    document.write("            <li><a href=\""+relativePath+"/index.html\">Home</a></li>");
    document.write("            <li><a href=\""+relativePath+"/preRequirements.html\">Pre-requirements</a></li>");
    document.write("            <li><a href=\""+relativePath+"/fileStructure.html\">File structure</a></li>");
    document.write("            <li><a href=\""+relativePath+"/develop/index.html\">Develop</a></li>");
    document.write("            <li><a href=\""+relativePath+"/develop/codingStyle.html\">&nbsp;&nbsp;&nbsp;Coding style</a></li>");
    document.write("            <li><a href=\""+relativePath+"/develop/plugins.html\">&nbsp;&nbsp;&nbsp;Plugins</a></li>");
    document.write("            <li><a href=\""+relativePath+"/develop/internationalisation.html\">&nbsp;&nbsp;&nbsp;Internationalisation</a></li>");
    document.write("            <li><a href=\""+relativePath+"/develop/tests.html\">&nbsp;&nbsp;&nbsp;Tests</a></li>");
    document.write("            <li><a href=\""+relativePath+"/buildRunAndTest.html\">Build, run and test</a></li>");
    document.write("            <li><a href=\""+relativePath+"/package.html\">Package</a></li>");
    document.write("            <li class=\"lastContentsMenuItem\"><a href=\""+relativePath+"/thirdPartyLibraries.html\">Third-party libraries</a></li>");
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
