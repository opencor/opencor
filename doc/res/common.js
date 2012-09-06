function doHeaderAndContentsMenu(pageName, relativePath, r, g, b, data) {
    document.write("<div class=\"header\">");
    document.write("    "+pageName);
    document.write("</div>");
    document.write("");
    document.write("<style>");
    document.write("    ul.contentsMenu {");
    document.write("        margin: 0px;");
    document.write("        position: fixed;");
    document.write("        top: 3px;");
    document.write("        right: 3px;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li {");
    document.write("        list-style: none;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul {");
    document.write("        visibility: hidden;");
    document.write("        margin: 0px;");
    document.write("        padding: 0px;");
    document.write("        position: fixed;");
    document.write("        top: 29px;");
    document.write("        right: 0px;");
    document.write("        background: rgb(249, 249, 249);");
    document.write("        border: 1px solid rgb("+r+", "+g+", "+b+");");
    document.write("        border-top-style: none;");
    document.write("        border-radius: 0px 0px 5px 5px;");
    document.write("        box-shadow: 0px 5px 5px -5px rgb(103, 103, 103);");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li {");
    document.write("        margin: 2px;");
    document.write("        font-size: 9pt;");
    document.write("        font-weight: normal;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li a {");
    document.write("        display: block;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li a:hover {");
    document.write("        background-color: rgba("+r+", "+g+", "+b+", 0.79);");
    document.write("        color: rgb(255, 255, 255);");
    document.write("        text-decoration: none;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li.lastContentsMenuItem a {");
    document.write("        border-radius: 0px 0px 5px 5px;");
    document.write("    }");
    document.write("</style>");
    document.write("");
    document.write("<ul class=\"contentsMenu\">");
    document.write("    <li>");
    document.write("        <img src=\""+relativePath+"/../res/pics/oxygen/actions/help-about.png\" width=24 height=24 alt=\"Contents\">");
    document.write("        <ul>");

    for (i = 0; i < data.length; ++i) {
        var indent = "";

        for (j = 0; j < data[i][0]; ++j)
            indent += "&nbsp;&nbsp;&nbsp;&nbsp;"

        if (i != data.length-1)
            document.write("            <li><a href=\""+relativePath+"/"+data[i][2]+"\">"+indent+data[i][1]+"</a></li>");
        else
            document.write("            <li class=\"lastContentsMenuItem\"><a href=\""+relativePath+"/"+data[i][2]+"\">"+indent+data[i][1]+"</a></li>");
    }

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

function copyright() {
    var date = new Date();

    document.write("<div class=\"copyright\">");
    document.write("    Copyright ©2011-"+date.getFullYear());
    document.write("</div>");
}
