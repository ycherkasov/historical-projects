#include <QApplication>
#include <QtGui>
#include <QTextEdit>
#include <QtPrintSupport/QPrinter>

// TODO: Line edit, passwords, editor

// QTextEdit could edit and save html
// QTextEdit+QTextDocumentWriter allows saving in txt. odf, html formats
// QTextEdit+QPrinter allows saving in PDF format
// QSyntaxHighlighter allows highlight syntax in QTextEdit (full-blown editor)
// QLineEdit may have buddy and enter passwords

int show_html_edit(int argc, char **argv){
    QApplication app(argc, argv);
    QTextEdit txt;

    // HTML is editable
    txt.setHtml("<HTML>"
        "<BODY BGCOLOR=MAGENTA>"
        "<CENTER><IMG SRC=\"agnetha.jpg\"></CENTER>"
        "<H2><CENTER>THE WINNER TAKES IT ALL</CENTER></H2>"
        "<H3><CENTER>(B.Andersson/B.Ulvaeus)</CENTER><H3>"
        "<FONT COLOR=BLUE>"
        "<P ALIGN=\"center\">"
        "<I>"
        "I don't wanna talk<BR>"
        "About the things we've gone through<BR>"
        "Though it's hurting me<BR>"
        "Now it's history<BR>"
        "I've played all my cards<BR>"
        "And that's what you've done too<BR>"
        "Nothing more to say<BR>"
        "No more ace to play<BR><BR>"
        "The winner takes it all<BR>"
        "The loser standing small<BR>"
        "Beside the victory<BR>"
        "That's her destiny<BR>"
        "..."
        "</I>"
        "</P>"
        "</FONT>" "</BODY>" "</HTML>"
        );
    txt.resize(600, 750);
    txt.show();
    return app.exec();

}

int show_save_plain_text(int argc, char **argv){
    QApplication app(argc, argv);
    QTextEdit txt("This is a text");
    QTextDocumentWriter writer;
    writer.setFormat("txt");
    writer.setFileName("exmapl.txt");
    writer.write(txt.document());
    txt.show();
    return app.exec();
}

int show_save_odf(int argc, char **argv){
    QApplication app(argc, argv);
    QTextEdit txt("This is a <b>text</b>");
    QTextDocumentWriter writer;
    writer.setFormat("odf");
    writer.setFileName("exmapl.odf");
    writer.write(txt.document());
    txt.show();
    return app.exec();
}

int show_save_html(int argc, char **argv){
    QApplication app(argc, argv);
    QTextEdit txt("This is a <B>text</B>");
    QTextDocumentWriter writer;
    writer.setFormat("html");
    writer.setFileName("exmapl.html");
    writer.write(txt.document());
    txt.show();
    return app.exec();
}

// Save in PDF a little different
int show_save_pdf(int argc, char **argv){
    QApplication app(argc, argv);
    QTextEdit txt("This is a <B>text</B>");
    QPrinter writer;
    writer.setOutputFormat(QPrinter::PdfFormat);
    writer.setOutputFileName("exmapl.pdf");
    txt.document()->print(&writer);
    txt.show();
    return app.exec();
}

int main(int argc, char **argv)
{
    //return show_html_edit(argc, argv);
    //return show_save_plain_text(argc, argv);
    //return show_save_odf(argc, argv);
    //return show_save_html(argc, argv);
    return show_save_pdf(argc, argv);
}
