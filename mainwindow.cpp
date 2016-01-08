#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QLabel>
#include <time.h>
#include <QTextStream>
#include <QPrinter>
#include <QFile>
#include <QtWebKitWidgets/QWebView>
#include "finddialog.h"
#include "findreplacedialog.h"
#include "preferences.h"
#include "highlighter.h"
#include "sundown/markdown.h"
#include "sundown/html.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int css=1;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), maxFileNr(10)
{
    imageLabel = new QLabel;
    QCoreApplication::setOrganizationName("Shieh");
    QCoreApplication::setOrganizationDomain("shieh.com");
    QCoreApplication::setApplicationName("WM");

    // setCentralWidget(imageLabel);

    ui->setupUi(this);
    //file
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(fnew()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(fopen()));
    // connect(ui->actionOpen_Recent,SIGNAL(triggered()),this,SLOT(fopenRecent()));
    connect(ui->actionClose,SIGNAL(clicked(bool)),this,SLOT(fclose()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(fsave()));
    connect(ui->actionExport,SIGNAL(triggered()),this,SLOT(fexport()));
    connect(ui->actionRename,SIGNAL(clicked(bool)),this,SLOT(frename()));
    //edit
    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(eundo()));
    connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(eredo()));
    connect(ui->actionDelete,SIGNAL(clicked(bool)),this,SLOT(edelete()));
    connect(ui->actionFind,SIGNAL(triggered()),this,SLOT(efind()));
    connect(ui->actionFind_Replace, SIGNAL(triggered()), this, SLOT(efindreplace()));
    //view
    connect(ui->actionLeft_right_1_1,SIGNAL(triggered()),this,SLOT(vlf_1_1()));
    connect(ui->actionLeft_right_1_2,SIGNAL(triggered()),this,SLOT(vlf_1_2()));
    connect(ui->actionLeft_right_2_1,SIGNAL(triggered()),this,SLOT(vlf_2_1()));
    //help
    connect(ui->actionMou_help,SIGNAL(triggered(bool)),this,SLOT(help()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));

    //WinMarker
    connect(ui->actionPreferences,SIGNAL(triggered()),this,SLOT(wpreferences()));
    //connect(ui->actionQuit,SIGNAL(clicked(bool)),this,SLOT(wquit()));

    ui->actionUndo->setShortcuts(QKeySequence::Undo);
    ui->actionRedo->setShortcuts(QKeySequence::Redo);

    //action
    connect(ui->actionCurrent_Date,SIGNAL(triggered()),this,SLOT(Current_Date()));
    connect(ui->actionCurrent_Time,SIGNAL(triggered()),this,SLOT(Current_Time()));
    connect(ui->action,SIGNAL(triggered()),this,SLOT(insertLeft()));
    connect(ui->action_2,SIGNAL(triggered()),this,SLOT(insertRight()));
    connect(ui->action_3,SIGNAL(triggered()),this,SLOT(insertAnd()));
    connect(ui->actionSpace,SIGNAL(triggered()),this,SLOT(insertSpace()));
    connect(ui->actionLink,SIGNAL(triggered()),this,SLOT(insertLink()));
    connect(ui->actionImage,SIGNAL(triggered()),this,SLOT(insertImage()));
    connect(ui->actionNew_Line,SIGNAL(triggered()),this,SLOT(insertLine()));
    connect(ui->actionComment,SIGNAL(triggered()),this,SLOT(insertComment()));
    connect(ui->actionInline_Code,SIGNAL(triggered()),this,SLOT(insertCode()));

    //listen keyboard
    ui->plainTextEdit->installEventFilter(this);

    change_theme(0);
    highlighter = new Highlighter(ui->plainTextEdit->document());

    createActionsAndConnections();

    m_findDialog = new FindDialog(this);
    m_findDialog->setModal(false);

    m_findReplaceDialog = new FindReplaceDialog(this);
    m_findReplaceDialog->setModal(false);

    m_preferences = new preferences(this);
    m_preferences->setModal(true);
    vlf_1_1();
    readSettings();
}

QString MainWindow::getHtml(const QString& str) {
    #define READ_UNIT 1024
    #define OUTPUT_UNIT 64
        struct buf *ob;
        struct sd_callbacks callbacks;
        struct html_renderopt options;
        struct sd_markdown *markdown;

        ob = bufnew(OUTPUT_UNIT);
        sdhtml_renderer(&callbacks, &options, 0);
        markdown = sd_markdown_new(MKDEXT_NO_INTRA_EMPHASIS|MKDEXT_TABLES|MKDEXT_AUTOLINK|MKDEXT_FENCED_CODE|MKDEXT_STRIKETHROUGH|MKDEXT_SUPERSCRIPT, 16, &callbacks, &options);

        sd_markdown_render(ob, (const uint8_t*)(str.toStdString().c_str()), str.toStdString().size(), markdown);
        sd_markdown_free(markdown);

        string str2((char*)ob->data,ob->size);

        bufrelease(ob);
        QString qstr = QString::fromStdString(str2);
        return qstr;
}

bool MainWindow::eventFilter (QObject *obj, QEvent *e )
{

    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *event = static_cast <QKeyEvent * > (e) ;
        QString html = ui->plainTextEdit->toPlainText();
        html = getHtml(html);
        QString css1 = "body { font-family: Helvetica, arial, sans-serif; font-size: 14px; line-height: 1.6; padding-top: 10px; padding-bottom: 10px; background-color: white; padding: 30px; } body > *:first-child { margin-top: 0 !important; } body > *:last-child { margin-bottom: 0 !important; } a { color: #4183C4; } a.absent { color: #cc0000; } a.anchor { display: block; padding-left: 30px; margin-left: -30px; cursor: pointer; position: absolute; top: 0; left: 0; bottom: 0; } h1, h2, h3, h4, h5, h6 { margin: 20px 0 10px; padding: 0; font-weight: bold; -webkit-font-smoothing: antialiased; cursor: text; position: relative; } h1:hover a.anchor, h2:hover a.anchor, h3:hover a.anchor, h4:hover a.anchor, h5:hover a.anchor, h6:hover a.anchor { background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA09pVFh0WE1MOmNvbS5hZG9iZS54bXAAAAAAADw/eHBhY2tldCBiZWdpbj0i77u/IiBpZD0iVzVNME1wQ2VoaUh6cmVTek5UY3prYzlkIj8+IDx4OnhtcG1ldGEgeG1sbnM6eD0iYWRvYmU6bnM6bWV0YS8iIHg6eG1wdGs9IkFkb2JlIFhNUCBDb3JlIDUuMy1jMDExIDY2LjE0NTY2MSwgMjAxMi8wMi8wNi0xNDo1NjoyNyAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RSZWY9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZVJlZiMiIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIENTNiAoMTMuMCAyMDEyMDMwNS5tLjQxNSAyMDEyLzAzLzA1OjIxOjAwOjAwKSAgKE1hY2ludG9zaCkiIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6OUM2NjlDQjI4ODBGMTFFMTg1ODlEODNERDJBRjUwQTQiIHhtcE1NOkRvY3VtZW50SUQ9InhtcC5kaWQ6OUM2NjlDQjM4ODBGMTFFMTg1ODlEODNERDJBRjUwQTQiPiA8eG1wTU06RGVyaXZlZEZyb20gc3RSZWY6aW5zdGFuY2VJRD0ieG1wLmlpZDo5QzY2OUNCMDg4MEYxMUUxODU4OUQ4M0REMkFGNTBBNCIgc3RSZWY6ZG9jdW1lbnRJRD0ieG1wLmRpZDo5QzY2OUNCMTg4MEYxMUUxODU4OUQ4M0REMkFGNTBBNCIvPiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gPD94cGFja2V0IGVuZD0iciI/PsQhXeAAAABfSURBVHjaYvz//z8DJYCRUgMYQAbAMBQIAvEqkBQWXI6sHqwHiwG70TTBxGaiWwjCTGgOUgJiF1J8wMRAIUA34B4Q76HUBelAfJYSA0CuMIEaRP8wGIkGMA54bgQIMACAmkXJi0hKJQAAAABJRU5ErkJggg==) no-repeat 10px center; text-decoration: none; } h1 tt, h1 code { font-size: inherit; } h2 tt, h2 code { font-size: inherit; } h3 tt, h3 code { font-size: inherit; } h4 tt, h4 code { font-size: inherit; } h5 tt, h5 code { font-size: inherit; } h6 tt, h6 code { font-size: inherit; } h1 { font-size: 28px; color: black; } h2 { font-size: 24px; border-bottom: 1px solid #cccccc; color: black; } h3 { font-size: 18px; } h4 { font-size: 16px; } h5 { font-size: 14px; } h6 { color: #777777; font-size: 14px; } p, blockquote, ul, ol, dl, li, table, pre { margin: 15px 0; } hr { background: transparent url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAYAAAAECAYAAACtBE5DAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAyJpVFh0WE1MOmNvbS5hZG9iZS54bXAAAAAAADw/eHBhY2tldCBiZWdpbj0i77u/IiBpZD0iVzVNME1wQ2VoaUh6cmVTek5UY3prYzlkIj8+IDx4OnhtcG1ldGEgeG1sbnM6eD0iYWRvYmU6bnM6bWV0YS8iIHg6eG1wdGs9IkFkb2JlIFhNUCBDb3JlIDUuMC1jMDYwIDYxLjEzNDc3NywgMjAxMC8wMi8xMi0xNzozMjowMCAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RSZWY9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZVJlZiMiIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIENTNSBNYWNpbnRvc2giIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6OENDRjNBN0E2NTZBMTFFMEI3QjRBODM4NzJDMjlGNDgiIHhtcE1NOkRvY3VtZW50SUQ9InhtcC5kaWQ6OENDRjNBN0I2NTZBMTFFMEI3QjRBODM4NzJDMjlGNDgiPiA8eG1wTU06RGVyaXZlZEZyb20gc3RSZWY6aW5zdGFuY2VJRD0ieG1wLmlpZDo4Q0NGM0E3ODY1NkExMUUwQjdCNEE4Mzg3MkMyOUY0OCIgc3RSZWY6ZG9jdW1lbnRJRD0ieG1wLmRpZDo4Q0NGM0E3OTY1NkExMUUwQjdCNEE4Mzg3MkMyOUY0OCIvPiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gPD94cGFja2V0IGVuZD0iciI/PqqezsUAAAAfSURBVHjaYmRABcYwBiM2QSA4y4hNEKYDQxAEAAIMAHNGAzhkPOlYAAAAAElFTkSuQmCC) repeat-x 0 0; border: 0 none; color: #cccccc; height: 4px; padding: 0; } body > h2:first-child { margin-top: 0; padding-top: 0; } body > h1:first-child { margin-top: 0; padding-top: 0; } body > h1:first-child + h2 { margin-top: 0; padding-top: 0; } body > h3:first-child, body > h4:first-child, body > h5:first-child, body > h6:first-child { margin-top: 0; padding-top: 0; } a:first-child h1, a:first-child h2, a:first-child h3, a:first-child h4, a:first-child h5, a:first-child h6 { margin-top: 0; padding-top: 0; } h1 p, h2 p, h3 p, h4 p, h5 p, h6 p { margin-top: 0; } li p.first { display: inline-block; } li { margin: 0; } ul, ol { padding-left: 30px; } ul :first-child, ol :first-child { margin-top: 0; } dl { padding: 0; } dl dt { font-size: 14px; font-weight: bold; font-style: italic; padding: 0; margin: 15px 0 5px; } dl dt:first-child { padding: 0; } dl dt > :first-child { margin-top: 0; } dl dt > :last-child { margin-bottom: 0; } dl dd { margin: 0 0 15px; padding: 0 15px; } dl dd > :first-child { margin-top: 0; } dl dd > :last-child { margin-bottom: 0; } blockquote { border-left: 4px solid #dddddd; padding: 0 15px; color: #777777; } blockquote > :first-child { margin-top: 0; } blockquote > :last-child { margin-bottom: 0; } table { padding: 0;border-collapse: collapse; } table tr { border-top: 1px solid #cccccc; background-color: white; margin: 0; padding: 0; } table tr:nth-child(2n) { background-color: #f8f8f8; } table tr th { font-weight: bold; border: 1px solid #cccccc; margin: 0; padding: 6px 13px; } table tr td { border: 1px solid #cccccc; margin: 0; padding: 6px 13px; } table tr th :first-child, table tr td :first-child { margin-top: 0; } table tr th :last-child, table tr td :last-child { margin-bottom: 0; } img { max-width: 100%; } span.frame { display: block; overflow: hidden; } span.frame > span { border: 1px solid #dddddd; display: block; float: left; overflow: hidden; margin: 13px 0 0; padding: 7px; width: auto; } span.frame span img { display: block; float: left; } span.frame span span { clear: both; color: #333333; display: block; padding: 5px 0 0; } span.align-center { display: block; overflow: hidden; clear: both; } span.align-center > span { display: block; overflow: hidden; margin: 13px auto 0; text-align: center; } span.align-center span img { margin: 0 auto; text-align: center; } span.align-right { display: block; overflow: hidden; clear: both; } span.align-right > span { display: block; overflow: hidden; margin: 13px 0 0; text-align: right; } span.align-right span img { margin: 0; text-align: right; } span.float-left { display: block; margin-right: 13px; overflow: hidden; float: left; } span.float-left span { margin: 13px 0 0; } span.float-right { display: block; margin-left: 13px; overflow: hidden; float: right; } span.float-right > span { display: block; overflow: hidden; margin: 13px auto 0; text-align: right; } code, tt { margin: 0 2px; padding: 0 5px; white-space: nowrap; border: 1px solid #eaeaea; background-color: #f8f8f8; border-radius: 3px; } pre code { margin: 0; padding: 0; white-space: pre; border: none; background: transparent; } .highlight pre { background-color: #f8f8f8; border: 1px solid #cccccc; font-size: 13px; line-height: 19px; overflow: auto; padding: 6px 10px; border-radius: 3px; } pre { background-color: #f8f8f8; border: 1px solid #cccccc; font-size: 13px; line-height: 19px; overflow: auto; padding: 6px 10px; border-radius: 3px; } pre code, pre tt { background-color: transparent; border: none; } sup { font-size: 0.83em; vertical-align: super; line-height: 0; } * { -webkit-print-color-adjust: exact; } @media screen and (min-width: 914px) { body { width: 854px; margin:0 auto; } } @media print { table, pre { page-break-inside: avoid; } pre { word-wrap: break-word; } }";
        QString css2 = "*{margin:0;padding:0;} body { font:13.34px helvetica,arial,freesans,clean,sans-serif; color:black; line-height:1.4em; background-color: #F8F8F8; padding: 0.7em; } p { margin:1em 0; line-height:1.5em; } table { font-size:inherit; font:100%; margin:1em; } table th{border-bottom:1px solid #bbb;padding:.2em 1em;} table td{border-bottom:1px solid #ddd;padding:.2em 1em;} input[type=text],input[type=password],input[type=image],textarea{font:99% helvetica,arial,freesans,sans-serif;} select,option{padding:0 .25em;} optgroup{margin-top:.5em;} pre,code{font:12px Monaco,'Courier New','DejaVu Sans Mono','Bitstream Vera Sans Mono',monospace;} pre { margin:1em 0; font-size:12px; background-color:#eee; border:1px solid #ddd; padding:5px; line-height:1.5em; color:#444; overflow:auto; -webkit-box-shadow:rgba(0,0,0,0.07) 0 1px 2px inset; -webkit-border-radius:3px; -moz-border-radius:3px;border-radius:3px; } pre code { padding:0; font-size:12px; background-color:#eee; border:none; } code { font-size:12px; background-color:#f8f8ff; color:#444; padding:0 .2em; border:1px solid #dedede; } img{border:0;max-width:100%;} abbr{border-bottom:none;} a{color:#4183c4;text-decoration:none;} a:hover{text-decoration:underline;} a code,a:link code,a:visited code{color:#4183c4;} h2,h3{margin:1em 0;} h1,h2,h3,h4,h5,h6{border:0;} h1{font-size:170%;border-top:4px solid #aaa;padding-top:.5em;margin-top:1.5em;} h1:first-child{margin-top:0;padding-top:.25em;border-top:none;} h2{font-size:150%;margin-top:1.5em;border-top:4px solid #e0e0e0;padding-top:.5em;} h3{margin-top:1em;} hr{border:1px solid #ddd;} ul{margin:1em 0 1em 2em;} ol{margin:1em 0 1em 2em;} ul li,ol li{margin-top:.5em;margin-bottom:.5em;} ul ul,ul ol,ol ol,ol ul{margin-top:0;margin-bottom:0;} blockquote{margin:1em 0;border-left:5px solid #ddd;padding-left:.6em;color:#555;} dt{font-weight:bold;margin-left:1em;} dd{margin-left:2em;margin-bottom:1em;} sup { font-size: 0.83em; vertical-align: super; line-height: 0; } * { -webkit-print-color-adjust: exact; } @media screen and (min-width: 914px) { body { width: 854px; margin:0 auto; } } @media print { table, pre { page-break-inside: avoid; } pre { word-wrap: break-word; } }";
        QString css3 = "h1, h2, h3, h4, h5, h6, p, blockquote { margin: 0; padding: 0; } body { font-family: 'Helvetica Neue', Helvetica, 'Hiragino Sans GB', Arial, sans-serif; font-size: 13px; line-height: 18px; color: #737373; background-color: white; margin: 10px 13px 10px 13px; } table { margin: 10px 0 15px 0; border-collapse: collapse; } td,th {	border: 1px solid #ddd; padding: 3px 10px; } th { padding: 5px 10px;	} a { color: #0069d6; } a:hover { color: #0050a3; text-decoration: none; } a img { border: none; } p { margin-bottom: 9px; } h1, h2, h3, h4, h5, h6 { color: #404040; line-height: 36px; } h1 { margin-bottom: 18px; font-size: 30px; } h2 { font-size: 24px; } h3 { font-size: 18px; } h4 { font-size: 16px; } h5 { font-size: 14px; } h6 { font-size: 13px; } hr { margin: 0 0 19px; border: 0; border-bottom: 1px solid #ccc; } blockquote { padding: 13px 13px 21px 15px; margin-bottom: 18px; font-family:georgia,serif; font-style: italic; } blockquote:before { content:'\201C'; font-size:40px; margin-left:-10px; font-family:georgia,serif; color:#eee; } blockquote p { font-size: 14px; font-weight: 300; line-height: 18px; margin-bottom: 0; font-style: italic; } code, pre { font-family: Monaco, Andale Mono, Courier New, monospace; } code { background-color: #fee9cc; color: rgba(0, 0, 0, 0.75); padding: 1px 3px; font-size: 12px; -webkit-border-radius: 3px; -moz-border-radius: 3px; border-radius: 3px; } pre { display: block; padding: 14px; margin: 0 0 18px; line-height: 16px; font-size: 11px; border: 1px solid #d9d9d9; white-space: pre-wrap; word-wrap: break-word; } pre code { background-color: #fff; color:#737373; font-size: 11px; padding: 0; } sup { font-size: 0.83em; vertical-align: super; line-height: 0; } * { -webkit-print-color-adjust: exact; } @media screen and (min-width: 914px) { body { width: 854px; margin:10px auto; } } @media print { body,code,pre code,h1,h2,h3,h4,h5,h6 { color: black; } table, pre { page-break-inside: avoid; } }";
        switch (css) {
            case 1: {
                ui->webView->setHtml("<head><style type='text/css'>"+css1+"</style></head>"+html);
                break;
            }
            case 2: {
                ui->webView->setHtml("<head><style type='text/css'>"+css2+"</style></head>"+html);
                break;
            }
            case 3: {
                ui->webView->setHtml("<head><style type='text/css'>"+css3+"</style></head>"+html);
                break;
            }

        }
        return false ;
    }
}

void MainWindow::Current_Date()
{
    time_t tt = time(NULL);//这句返回的只是一个时间cuo
    tm* t= localtime(&tt);
    char year[10],mon[10],day[10];
    itoa(t->tm_year + 1900, year, 10);
    itoa(t->tm_mon + 1, mon, 10);
    itoa(t->tm_mday , day, 10);
    string syear(year);
    string smon(mon);
    string sday(day);
    ui->plainTextEdit->appendPlainText(QString::fromStdString(syear+"-"+smon+"-"+sday));
}

void MainWindow::Current_Time()
{
    time_t tt = time(NULL);//这句返回的只是一个时间cuo
    tm* t= localtime(&tt);
    char hour[10],min[10],sec[10];
    itoa( t->tm_hour, hour, 10);
    itoa(t->tm_min, min, 10);
    itoa(t->tm_sec , sec, 10);
    string shour(hour);
    string smin(min);
    string ssec(sec);
    ui->plainTextEdit->appendPlainText(QString::fromStdString(shour+":"+smin+":"+ssec));
    change_css(css);
}

void MainWindow::insertLeft()
{
    ui->plainTextEdit->appendPlainText("&lt;");
    change_css(css);
}

void MainWindow::insertRight()
{
    ui->plainTextEdit->appendPlainText("&gt;");
    change_css(css);
}

void MainWindow::insertAnd()
{
    ui->plainTextEdit->appendPlainText("&amp;");
    change_css(css);
}

void MainWindow::insertSpace()
{
    ui->plainTextEdit->appendPlainText("&nbsp;");
    change_css(css);
}

void MainWindow::insertLink()
{
    ui->plainTextEdit->appendPlainText("[&nbsp;](http://)");
    change_css(css);
}

void MainWindow::insertImage()
{
    ui->plainTextEdit->appendPlainText("![image](http://)");
    change_css(css);
}

void MainWindow::insertLine()
{
    ui->plainTextEdit->appendPlainText("\n");
    change_css(css);
}

void MainWindow::insertComment()
{
    ui->plainTextEdit->appendPlainText("<!---->");
    change_css(css);
}

void MainWindow::insertCode()
{
    ui->plainTextEdit->appendPlainText("``");
    change_css(css);
}

void MainWindow::help()
{

}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}


void MainWindow::writeSettings() {
    QSettings settings;
    m_findDialog->writeSettings(settings);
    m_findReplaceDialog->writeSettings(settings);
    m_preferences->writeSettings(settings);
}

void MainWindow::readSettings() {
    QSettings settings;
    m_findDialog->readSettings(settings);
    m_findReplaceDialog->readSettings(settings);
    m_preferences->readSettings(settings);
}


void MainWindow::change_theme(int theme) {

    switch (theme) {
        case 0: {
            ui->plainTextEdit->setStyleSheet("background: white;color: black;font-size: 15px;");
            break;
        }
        case 1: {
            ui->plainTextEdit->setStyleSheet("background: white;color: black;font-size: 22px;");
            break;
        }
        case 2: {
            ui->plainTextEdit->setStyleSheet("background: black;color: white;font-size: 15px;");
            break;
        }
        case 3: {
            ui->plainTextEdit->setStyleSheet("background: black;color: white;font-size: 22px;");
            break;
        }
        case 4: {
            ui->plainTextEdit->setStyleSheet("background: white;color: blue;");
            break;
        }
    }

    return;
}

void MainWindow::change_css(int theme) {
    css = theme;
    QString html = ui->plainTextEdit->toPlainText();
    html = getHtml(html);
    QString css1 = "body { font-family: Helvetica, arial, sans-serif; font-size: 14px; line-height: 1.6; padding-top: 10px; padding-bottom: 10px; background-color: white; padding: 30px; } body > *:first-child { margin-top: 0 !important; } body > *:last-child { margin-bottom: 0 !important; } a { color: #4183C4; } a.absent { color: #cc0000; } a.anchor { display: block; padding-left: 30px; margin-left: -30px; cursor: pointer; position: absolute; top: 0; left: 0; bottom: 0; } h1, h2, h3, h4, h5, h6 { margin: 20px 0 10px; padding: 0; font-weight: bold; -webkit-font-smoothing: antialiased; cursor: text; position: relative; } h1:hover a.anchor, h2:hover a.anchor, h3:hover a.anchor, h4:hover a.anchor, h5:hover a.anchor, h6:hover a.anchor { background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA09pVFh0WE1MOmNvbS5hZG9iZS54bXAAAAAAADw/eHBhY2tldCBiZWdpbj0i77u/IiBpZD0iVzVNME1wQ2VoaUh6cmVTek5UY3prYzlkIj8+IDx4OnhtcG1ldGEgeG1sbnM6eD0iYWRvYmU6bnM6bWV0YS8iIHg6eG1wdGs9IkFkb2JlIFhNUCBDb3JlIDUuMy1jMDExIDY2LjE0NTY2MSwgMjAxMi8wMi8wNi0xNDo1NjoyNyAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RSZWY9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZVJlZiMiIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIENTNiAoMTMuMCAyMDEyMDMwNS5tLjQxNSAyMDEyLzAzLzA1OjIxOjAwOjAwKSAgKE1hY2ludG9zaCkiIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6OUM2NjlDQjI4ODBGMTFFMTg1ODlEODNERDJBRjUwQTQiIHhtcE1NOkRvY3VtZW50SUQ9InhtcC5kaWQ6OUM2NjlDQjM4ODBGMTFFMTg1ODlEODNERDJBRjUwQTQiPiA8eG1wTU06RGVyaXZlZEZyb20gc3RSZWY6aW5zdGFuY2VJRD0ieG1wLmlpZDo5QzY2OUNCMDg4MEYxMUUxODU4OUQ4M0REMkFGNTBBNCIgc3RSZWY6ZG9jdW1lbnRJRD0ieG1wLmRpZDo5QzY2OUNCMTg4MEYxMUUxODU4OUQ4M0REMkFGNTBBNCIvPiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gPD94cGFja2V0IGVuZD0iciI/PsQhXeAAAABfSURBVHjaYvz//z8DJYCRUgMYQAbAMBQIAvEqkBQWXI6sHqwHiwG70TTBxGaiWwjCTGgOUgJiF1J8wMRAIUA34B4Q76HUBelAfJYSA0CuMIEaRP8wGIkGMA54bgQIMACAmkXJi0hKJQAAAABJRU5ErkJggg==) no-repeat 10px center; text-decoration: none; } h1 tt, h1 code { font-size: inherit; } h2 tt, h2 code { font-size: inherit; } h3 tt, h3 code { font-size: inherit; } h4 tt, h4 code { font-size: inherit; } h5 tt, h5 code { font-size: inherit; } h6 tt, h6 code { font-size: inherit; } h1 { font-size: 28px; color: black; } h2 { font-size: 24px; border-bottom: 1px solid #cccccc; color: black; } h3 { font-size: 18px; } h4 { font-size: 16px; } h5 { font-size: 14px; } h6 { color: #777777; font-size: 14px; } p, blockquote, ul, ol, dl, li, table, pre { margin: 15px 0; } hr { background: transparent url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAYAAAAECAYAAACtBE5DAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAyJpVFh0WE1MOmNvbS5hZG9iZS54bXAAAAAAADw/eHBhY2tldCBiZWdpbj0i77u/IiBpZD0iVzVNME1wQ2VoaUh6cmVTek5UY3prYzlkIj8+IDx4OnhtcG1ldGEgeG1sbnM6eD0iYWRvYmU6bnM6bWV0YS8iIHg6eG1wdGs9IkFkb2JlIFhNUCBDb3JlIDUuMC1jMDYwIDYxLjEzNDc3NywgMjAxMC8wMi8xMi0xNzozMjowMCAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RSZWY9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZVJlZiMiIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIENTNSBNYWNpbnRvc2giIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6OENDRjNBN0E2NTZBMTFFMEI3QjRBODM4NzJDMjlGNDgiIHhtcE1NOkRvY3VtZW50SUQ9InhtcC5kaWQ6OENDRjNBN0I2NTZBMTFFMEI3QjRBODM4NzJDMjlGNDgiPiA8eG1wTU06RGVyaXZlZEZyb20gc3RSZWY6aW5zdGFuY2VJRD0ieG1wLmlpZDo4Q0NGM0E3ODY1NkExMUUwQjdCNEE4Mzg3MkMyOUY0OCIgc3RSZWY6ZG9jdW1lbnRJRD0ieG1wLmRpZDo4Q0NGM0E3OTY1NkExMUUwQjdCNEE4Mzg3MkMyOUY0OCIvPiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gPD94cGFja2V0IGVuZD0iciI/PqqezsUAAAAfSURBVHjaYmRABcYwBiM2QSA4y4hNEKYDQxAEAAIMAHNGAzhkPOlYAAAAAElFTkSuQmCC) repeat-x 0 0; border: 0 none; color: #cccccc; height: 4px; padding: 0; } body > h2:first-child { margin-top: 0; padding-top: 0; } body > h1:first-child { margin-top: 0; padding-top: 0; } body > h1:first-child + h2 { margin-top: 0; padding-top: 0; } body > h3:first-child, body > h4:first-child, body > h5:first-child, body > h6:first-child { margin-top: 0; padding-top: 0; } a:first-child h1, a:first-child h2, a:first-child h3, a:first-child h4, a:first-child h5, a:first-child h6 { margin-top: 0; padding-top: 0; } h1 p, h2 p, h3 p, h4 p, h5 p, h6 p { margin-top: 0; } li p.first { display: inline-block; } li { margin: 0; } ul, ol { padding-left: 30px; } ul :first-child, ol :first-child { margin-top: 0; } dl { padding: 0; } dl dt { font-size: 14px; font-weight: bold; font-style: italic; padding: 0; margin: 15px 0 5px; } dl dt:first-child { padding: 0; } dl dt > :first-child { margin-top: 0; } dl dt > :last-child { margin-bottom: 0; } dl dd { margin: 0 0 15px; padding: 0 15px; } dl dd > :first-child { margin-top: 0; } dl dd > :last-child { margin-bottom: 0; } blockquote { border-left: 4px solid #dddddd; padding: 0 15px; color: #777777; } blockquote > :first-child { margin-top: 0; } blockquote > :last-child { margin-bottom: 0; } table { padding: 0;border-collapse: collapse; } table tr { border-top: 1px solid #cccccc; background-color: white; margin: 0; padding: 0; } table tr:nth-child(2n) { background-color: #f8f8f8; } table tr th { font-weight: bold; border: 1px solid #cccccc; margin: 0; padding: 6px 13px; } table tr td { border: 1px solid #cccccc; margin: 0; padding: 6px 13px; } table tr th :first-child, table tr td :first-child { margin-top: 0; } table tr th :last-child, table tr td :last-child { margin-bottom: 0; } img { max-width: 100%; } span.frame { display: block; overflow: hidden; } span.frame > span { border: 1px solid #dddddd; display: block; float: left; overflow: hidden; margin: 13px 0 0; padding: 7px; width: auto; } span.frame span img { display: block; float: left; } span.frame span span { clear: both; color: #333333; display: block; padding: 5px 0 0; } span.align-center { display: block; overflow: hidden; clear: both; } span.align-center > span { display: block; overflow: hidden; margin: 13px auto 0; text-align: center; } span.align-center span img { margin: 0 auto; text-align: center; } span.align-right { display: block; overflow: hidden; clear: both; } span.align-right > span { display: block; overflow: hidden; margin: 13px 0 0; text-align: right; } span.align-right span img { margin: 0; text-align: right; } span.float-left { display: block; margin-right: 13px; overflow: hidden; float: left; } span.float-left span { margin: 13px 0 0; } span.float-right { display: block; margin-left: 13px; overflow: hidden; float: right; } span.float-right > span { display: block; overflow: hidden; margin: 13px auto 0; text-align: right; } code, tt { margin: 0 2px; padding: 0 5px; white-space: nowrap; border: 1px solid #eaeaea; background-color: #f8f8f8; border-radius: 3px; } pre code { margin: 0; padding: 0; white-space: pre; border: none; background: transparent; } .highlight pre { background-color: #f8f8f8; border: 1px solid #cccccc; font-size: 13px; line-height: 19px; overflow: auto; padding: 6px 10px; border-radius: 3px; } pre { background-color: #f8f8f8; border: 1px solid #cccccc; font-size: 13px; line-height: 19px; overflow: auto; padding: 6px 10px; border-radius: 3px; } pre code, pre tt { background-color: transparent; border: none; } sup { font-size: 0.83em; vertical-align: super; line-height: 0; } * { -webkit-print-color-adjust: exact; } @media screen and (min-width: 914px) { body { width: 854px; margin:0 auto; } } @media print { table, pre { page-break-inside: avoid; } pre { word-wrap: break-word; } }";
    QString css2 = "*{margin:0;padding:0;} body { font:13.34px helvetica,arial,freesans,clean,sans-serif; color:black; line-height:1.4em; background-color: #F8F8F8; padding: 0.7em; } p { margin:1em 0; line-height:1.5em; } table { font-size:inherit; font:100%; margin:1em; } table th{border-bottom:1px solid #bbb;padding:.2em 1em;} table td{border-bottom:1px solid #ddd;padding:.2em 1em;} input[type=text],input[type=password],input[type=image],textarea{font:99% helvetica,arial,freesans,sans-serif;} select,option{padding:0 .25em;} optgroup{margin-top:.5em;} pre,code{font:12px Monaco,'Courier New','DejaVu Sans Mono','Bitstream Vera Sans Mono',monospace;} pre { margin:1em 0; font-size:12px; background-color:#eee; border:1px solid #ddd; padding:5px; line-height:1.5em; color:#444; overflow:auto; -webkit-box-shadow:rgba(0,0,0,0.07) 0 1px 2px inset; -webkit-border-radius:3px; -moz-border-radius:3px;border-radius:3px; } pre code { padding:0; font-size:12px; background-color:#eee; border:none; } code { font-size:12px; background-color:#f8f8ff; color:#444; padding:0 .2em; border:1px solid #dedede; } img{border:0;max-width:100%;} abbr{border-bottom:none;} a{color:#4183c4;text-decoration:none;} a:hover{text-decoration:underline;} a code,a:link code,a:visited code{color:#4183c4;} h2,h3{margin:1em 0;} h1,h2,h3,h4,h5,h6{border:0;} h1{font-size:170%;border-top:4px solid #aaa;padding-top:.5em;margin-top:1.5em;} h1:first-child{margin-top:0;padding-top:.25em;border-top:none;} h2{font-size:150%;margin-top:1.5em;border-top:4px solid #e0e0e0;padding-top:.5em;} h3{margin-top:1em;} hr{border:1px solid #ddd;} ul{margin:1em 0 1em 2em;} ol{margin:1em 0 1em 2em;} ul li,ol li{margin-top:.5em;margin-bottom:.5em;} ul ul,ul ol,ol ol,ol ul{margin-top:0;margin-bottom:0;} blockquote{margin:1em 0;border-left:5px solid #ddd;padding-left:.6em;color:#555;} dt{font-weight:bold;margin-left:1em;} dd{margin-left:2em;margin-bottom:1em;} sup { font-size: 0.83em; vertical-align: super; line-height: 0; } * { -webkit-print-color-adjust: exact; } @media screen and (min-width: 914px) { body { width: 854px; margin:0 auto; } } @media print { table, pre { page-break-inside: avoid; } pre { word-wrap: break-word; } }";
    QString css3 = "h1, h2, h3, h4, h5, h6, p, blockquote { margin: 0; padding: 0; } body { font-family: 'Helvetica Neue', Helvetica, 'Hiragino Sans GB', Arial, sans-serif; font-size: 13px; line-height: 18px; color: #737373; background-color: white; margin: 10px 13px 10px 13px; } table { margin: 10px 0 15px 0; border-collapse: collapse; } td,th {	border: 1px solid #ddd; padding: 3px 10px; } th { padding: 5px 10px;	} a { color: #0069d6; } a:hover { color: #0050a3; text-decoration: none; } a img { border: none; } p { margin-bottom: 9px; } h1, h2, h3, h4, h5, h6 { color: #404040; line-height: 36px; } h1 { margin-bottom: 18px; font-size: 30px; } h2 { font-size: 24px; } h3 { font-size: 18px; } h4 { font-size: 16px; } h5 { font-size: 14px; } h6 { font-size: 13px; } hr { margin: 0 0 19px; border: 0; border-bottom: 1px solid #ccc; } blockquote { padding: 13px 13px 21px 15px; margin-bottom: 18px; font-family:georgia,serif; font-style: italic; } blockquote:before { content:'\201C'; font-size:40px; margin-left:-10px; font-family:georgia,serif; color:#eee; } blockquote p { font-size: 14px; font-weight: 300; line-height: 18px; margin-bottom: 0; font-style: italic; } code, pre { font-family: Monaco, Andale Mono, Courier New, monospace; } code { background-color: #fee9cc; color: rgba(0, 0, 0, 0.75); padding: 1px 3px; font-size: 12px; -webkit-border-radius: 3px; -moz-border-radius: 3px; border-radius: 3px; } pre { display: block; padding: 14px; margin: 0 0 18px; line-height: 16px; font-size: 11px; border: 1px solid #d9d9d9; white-space: pre-wrap; word-wrap: break-word; } pre code { background-color: #fff; color:#737373; font-size: 11px; padding: 0; } sup { font-size: 0.83em; vertical-align: super; line-height: 0; } * { -webkit-print-color-adjust: exact; } @media screen and (min-width: 914px) { body { width: 854px; margin:10px auto; } } @media print { body,code,pre code,h1,h2,h3,h4,h5,h6 { color: black; } table, pre { page-break-inside: avoid; } }";
    switch (css) {
        case 0: {
            ui->webView->setHtml("<head><style type='text/css'>"+css1+"</style></head>"+html);
            break;
        }
        case 1: {
            ui->webView->setHtml("<head><style type='text/css'>"+css2+"</style></head>"+html);
            break;
        }
        case 2: {
            ui->webView->setHtml("<head><style type='text/css'>"+css3+"</style></head>"+html);
            break;
        }

    }

    return;
}

void MainWindow::fnew()
{
    fsave();
}


void MainWindow::fopen()
{
    QString filePath = QFileDialog::getOpenFileName(
                       this, tr("Open File"), "",
                       tr("text files (*.md *.txt)"));
    if (!filePath.isEmpty()) {
        loadFile(filePath);
    }
}


void MainWindow::loadFile(const QString &filePath){
    QFile inputFile(filePath);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          ui->plainTextEdit->appendPlainText(line);
       }
       inputFile.close();
    }
    this->setWindowTitle(filePath);

    adjustForCurrentFile(filePath);
}


void MainWindow::fopenRecent(){
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        loadFile(action->data().toString());
    }
}


void MainWindow::createActionsAndConnections(){
    // ui->actionOpen->setShortcut(QKeySequence::Open);
    QAction* recentFileAction = 0;
    for(int i = 0; i < maxFileNr; i++){
        recentFileAction = new QAction(this);
        recentFileAction->setVisible(false);
        QObject::connect(recentFileAction, SIGNAL(triggered()),
                                     this, SLOT(fopenRecent()));
        recentFileActionList.append(recentFileAction);
    }

    for(int i = 0; i < maxFileNr; i++) {
        ui->menu_Open_Recent->addAction(recentFileActionList.at(i));
    }
    updateRecentActionList();
}


void MainWindow::updateRecentActionList(){
    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();

    int itEnd = 0;
    if(recentFilePaths.size() <= maxFileNr) {
        itEnd = recentFilePaths.size();
    }
    else {
        itEnd = maxFileNr;
    }

    for (int i = 0; i < itEnd; i++) {
        QString strippedName = QFileInfo(recentFilePaths.at(i)).fileName();
        recentFileActionList.at(i)->setText(strippedName);
        recentFileActionList.at(i)->setData(recentFilePaths.at(i));
        recentFileActionList.at(i)->setVisible(true);
    }

    for (int i = itEnd; i < maxFileNr; i++) {
        recentFileActionList.at(i)->setVisible(false);
    }
}


void MainWindow::adjustForCurrentFile(const QString &filePath){
    currentFilePath = filePath;
    setWindowFilePath(currentFilePath);

    QSettings settings;
    QStringList recentFilePaths =
            settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(filePath);
    recentFilePaths.prepend(filePath);
    while (recentFilePaths.size() > maxFileNr)
        recentFilePaths.removeLast();
    settings.setValue("recentFiles", recentFilePaths);

    // see note
    updateRecentActionList();
}


void MainWindow::fsave() {
    QString filePath = QFileDialog::getSaveFileName(
                       this, tr("Save File"), "",
                       tr("text files (*.md *.txt)"));
    if (!filePath.isEmpty()) {
        saveFile(filePath);
    }

    return;
}


void MainWindow::fexport() {
    QString filePath = QFileDialog::getSaveFileName(
                       this, tr("Save File"), "",
                       tr("pdf files (*.pdf)"));
    if (!filePath.isEmpty()) {
        savePdf(filePath);
    }

    return;
}


void MainWindow::saveFile(const QString &filePath) {
    QFile f(filePath);

    f.open( QIODevice::WriteOnly );

    f.write(ui->plainTextEdit->toPlainText().toUtf8());

    f.close();

    return;
}

void MainWindow::savePdf(const QString &filePath) {
    QString html=getHtml(ui->plainTextEdit->toPlainText());
    html="<head><style type='text/css'>h2{color:red;}</style></head>"+html;
    QPrinter printer_html;
    printer_html.setPageSize(QPrinter::A4);
    printer_html.setOutputFormat(QPrinter::PdfFormat);
    printer_html.setOutputFileName(filePath);
    QTextDocument text_document;
    text_document.setHtml(html);
    text_document.print(&printer_html);
    text_document.end();
}

void MainWindow::eundo() {
    ui->plainTextEdit->undo();

    return;
}

void MainWindow::eredo() {
    ui->plainTextEdit->redo();

    return;
}


void MainWindow::efind() {
    m_findDialog->show();
    m_findDialog->setTextEdit(ui->plainTextEdit);

    return;
}


void MainWindow::efindreplace() {
    m_findReplaceDialog->show();
    m_findReplaceDialog->setTextEdit(ui->plainTextEdit);

    return;
}


void MainWindow::vlf_x_y(int x, int y) {
    ui->horizontalLayout_1->setStretch(x, y);

    return;
}


void MainWindow::vlf_1_1() {
    vlf_x_y(0, 1);
    vlf_x_y(1, 1);

    return;
}


void MainWindow::vlf_2_1() {
    vlf_x_y(0, 2);
    vlf_x_y(1, 1);
    return;
}


void MainWindow::vlf_1_2() {
    vlf_x_y(0, 1);
    vlf_x_y(1, 2);

    return;
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About WM"),
                       tr("About WM."));
}

//new function interface
//functions in preference to be realised
void MainWindow::wpreferences()
{
    m_preferences->show();
    return;
}


MainWindow::~MainWindow()
{
    delete ui;
    delete m_findDialog;
    delete m_findReplaceDialog;
    delete m_preferences;
}
