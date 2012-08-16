function initContentsMenu(relativePath, r, g, b) {
    document.write("<style>");
    document.write("ul.contentsMenu {");
    document.write("    margin: 0px;");
    document.write("    position: fixed;");
    document.write("    top: 3px;");
    document.write("    right: 3px;");
    document.write("}");
    document.write("");
    document.write("ul.contentsMenu li {");
    document.write("    list-style: none;");
    document.write("}");
    document.write("");
    document.write("ul.contentsMenu li ul {");
    document.write("    visibility: hidden;");
    document.write("    margin: 0px;");
    document.write("    padding: 0px;");
    document.write("    position: fixed;");
    document.write("    top: 29px;");
    document.write("    right: 0px;");
    document.write("    background: rgb(249, 249, 249);");
    document.write("    border: 1px solid rgb("+r+", "+g+", "+b+");");
    document.write("    border-top-style: none;");
    document.write("    border-radius: 0px 0px 5px 5px;");
    document.write("    box-shadow: 0px 5px 5px -5px rgb(103, 103, 103);");
    document.write("}");
    document.write("");
    document.write("ul.contentsMenu li ul li {");
    document.write("    margin: 2px;");
    document.write("    font-size: 9pt;");
    document.write("    font-weight: normal;");
    document.write("}");
    document.write("");
    document.write("ul.contentsMenu li ul li a {");
    document.write("    display: block;");
    document.write("}");
    document.write("");
    document.write("ul.contentsMenu li ul li a:hover {");
    document.write("    background-color: rgba("+r+", "+g+", "+b+", 0.79);");
    document.write("    color: rgb(255, 255, 255);");
    document.write("    text-decoration: none;");
    document.write("}");
    document.write("");
    document.write("ul.contentsMenu li ul li.lastContentsMenuItem a {");
    document.write("    border-radius: 0px 0px 5px 5px;");
    document.write("}");
    document.write("</style>");
}

function copyright() {
    var date = new Date();

    document.write("Copyright ©2011-"+date.getFullYear());
}
