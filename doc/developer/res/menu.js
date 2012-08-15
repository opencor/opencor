function showContentsMenu(relativePath) {
    document.write("<style>");
    document.write("    img.contentsMenu {");
    document.write("        position: fixed;");
    document.write("        right: 3px;");
    document.write("    }");
    document.write("</style>");

    document.write("<img class=\"contentsMenu\" src=\""+relativePath+"/../res/pics/oxygen/actions/help-contents.png\" width=24 height=24 alt=\"Contents\">");
}
