/*
* Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
*
*/


#ifndef SIDEBARCLIPBOARDPLUGIN_H
#define SIDEBARCLIPBOARDPLUGIN_H

#include <QGenericPlugin>
#include <QObject>
#include <QHash>
#include <QDebug>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMouseEvent>
#include <QClipboard>
#include <QMimeData>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QStaticText>
#include <QColor>
#include <QLabel>
#include <QByteArray>
#include <QBitmap>
#include <QFileInfo>
#include "clipboardpluginiface.h"
#include "clipboardwidgetentry.h"
#include "sidebarclipboardsignal.h"
#include "searchwidgetitemcontent.h"
#include "editorwidget.h"
#include "clipboardsignal.h"
#include "clipboardlisetwidget.h"
#include "cleanpromptbox.h"
#include "clipBoardInternalSignal.h"
//#include "clipbaordstructoriginaldata.h"
#define  WIDGET_ENTRY_COUNT 10
#define  SIDEBAR_CLIPBOARD_QSS_PATH  ":/qss/sidebarClipboard.css"
enum fileType {
    Txt,
    Svg,
    Png,
    Bmp,
    Xml,
    Docx,
    Pptx,
    Xlsx,
    Zip,
    Pdf,
    Pro,
    Esle
};
typedef struct  clipboardOriginalDataHash {
    ClipboardWidgetEntry* WidgetEntry;
    const QMimeData* MimeData;
    QPixmap          *p_pixmap;
    QString          text;
    QString          Clipbaordformat;
    QList<QUrl>      urls;
    int              Sequence;
//    int              DataFlag = 0;  //数据标记，判断该数据类型为Pixmap还是Text DataFlag = 1 --> 文本, DataFlag = 2 --> 图片
} OriginalDataHashValue;

//static SidebarClipboardPlugin *global_instance = nullptr;
class SidebarClipboardPlugin:public QObject, public ClipboardInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.ukui.sidebar-qt.plugin-iface.ClipboardInterface" FILE "clipboardPlugin.json")
    Q_INTERFACES(ClipboardInterface)

public:
    SidebarClipboardPlugin(QObject *parent = 0);
    //plugin implement 统一接口
    const QString name() override {return QObject::tr("ClipBoard");}
    PluginType pluginType() override {return PluginType::ClipBoard;}
    const QString description() override {return QObject::tr("Show the folder children as icons.");}
    const QIcon icon() override {return QIcon::fromTheme("view-grid-symbolic", QIcon::fromTheme("folder"));}
    void setEnable(bool enable) override {Q_UNUSED(enable)}
    bool isEnable() override {return true;}

    /* 剪贴板接口 */
    virtual QListWidget* getClipbaordListWidget() override;
    virtual SidebarClipBoardSignal* createClipSignal() override;
    virtual QWidget* getClipbaordGroupBox() override;
    //    static SidebarClipboardPlugin *getInstance();

    /* 剪贴板成员和成员函数 */
    QHash<QListWidgetItem*, OriginalDataHashValue*> m_pClipboardDataHash;
    QList<OriginalDataHashValue*> m_ListClipboardData;
    QListWidget *m_pShortcutOperationListWidget;
    QListWidget *m_pSearchWidgetListWidget;
    QGroupBox   *m_pSidebarClipboardBox;
    QWidget     *m_pSidebarClipboardWidget;
    QLabel      *m_pSideBarClipboardLable;
    QStringList  m_fileSuffix;
    bool         m_bPromptBoxBool;
    bool         m_bsortEntryBool = true;

    QVBoxLayout     *m_pClipboardLaout;
    QClipboard      *m_pSidebarClipboard;
    ClipboardSignal *m_pClipSignal;
    SearchWidgetItemContent *m_pSearchArea;
    QMimeData mineData;

    QTranslator *translator;

    /* 注册Widget界面和Item/lable/剪贴板数据/的关系 */
    void registerWidgetOriginalDataHash(QListWidgetItem *key, OriginalDataHashValue *value);
    OriginalDataHashValue *GetOriginalDataValue(QListWidgetItem *key);
    void removeOriginalDataHash(QListWidgetItem *key);
    void sortingEntrySequence();

    /* 对m_ListClipboardData链表的操作 */
    void inserOriginalDataList(OriginalDataHashValue *value);
    void removeOriginalDataList(OriginalDataHashValue *value);
    int  ItertionOriginalDataList(OriginalDataHashValue *value);
    void moveOriginalDataFirstList(OriginalDataHashValue *value);

    //void FixOriginalDataFirstList(OriginalDataHashValue *value, QString text, const QMimeData mimedata);



    void removeLastWidgetItem();                                            /* 限制复制条数 */
    bool booleanExistWidgetItem(QString Text);                              /* 判断在ListWidget是否存在，如果不存在则返回fasle，创建，返回true，不创建 */
    void createFindClipboardWidgetItem();                                   /* 创建查找条目 */
    void WhetherTopFirst();                                                 /* 设置新置顶的条目写入到剪贴版中去 */
    void connectWidgetEntryButton(ClipboardWidgetEntry * w);                /* 连接WIdgetEntry条目中三个按钮的槽函数 */
    void createTipLable();                                                  /* 创建无剪贴板板字样 */
    void createWidget();                                                    /* 创建剪贴板主Widget和搜索栏与条目的ListWidget界面 */
    void sortingEntryShow();                                                /* 將條目有序的展現出來 */
    bool booleanExistWidgetImagin(QPixmap Pixmap);
    void AddfileSuffix();
    QIcon fileSuffixGetsIcon(QString Url);
    QString SetFormatBody(QString text, ClipboardWidgetEntry *w);           /* 设置... */
    QString judgeBlankLine(QStringList list);                               /* 去除掉空行，显示有字体的行 */
    bool    substringSposition(QString formatBody, QStringList list);       /* 判断后面是否还有子串 */
    QListWidgetItem* iterationClipboardDataHash(ClipboardWidgetEntry *w);   /* 迭代Hash表m_pClipboardDataHash */
    int iterationDataHashSearchSequence(int Index);                         /* 迭代Hash表查找其中的当前下标是否存在 */
    QMimeData *copyMinedata(const QMimeData* mimeReference);                /* 拷贝QMimeData拷贝数据类型 */
    QMimeData *structureQmimeDate(OriginalDataHashValue *value);            /* 构造一个QMimeDate类型数据 */

signals:
    void Itemchange();
    void EditConfirmButtonSignal(ClipboardWidgetEntry *, EditorWidget*);

public slots:
    void createWidgetEntry();
    void popButtonSlots(ClipboardWidgetEntry *w);
    void editButtonSlots(ClipboardWidgetEntry *w);
    void removeButtonSlots(ClipboardWidgetEntry *w);
    void removeAllWidgetItem();
    void searchClipboardLableTextSlots(QString Text);
    void ItemNumchagedSlots();
};

#endif // SIDEBARCLIPBOARDPLUGIN_H
