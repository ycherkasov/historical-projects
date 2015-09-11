#include <QApplication>
#include <QtGui>
#include <QTextEdit>

// QTextEdit could edit and save html

int main(int argc, char **argv)
{
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
); txt.resize(320, 250);
txt.show();
return app.exec(); 
}
