#include <QApplication>
#include <QTabWidget>
#include <QToolBox>
#include <QListWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLayout>

#include "toggle_list_styles.h"

// * all listable Widgets (lists, comboboxes etc) support interting string and range
// insertItem()/InsertItems()
// * List element could be icon, string with icon, and any widget
// * List styles support noSelection/SingleSelection/Edit/Drag/Drop etc
// * All listable items support method ex. sortItems(Qt::AscendingOrder);
// * TreeWidget has checkboxes style
// * TreeWidget also could contain widgets
// * tree->setSortingEnabled(true) allow sorting by click on the header
// * List/Table items are clonnable
// * list elements could be editable (depend on style). QValidator could be used for input check


// TODO:
// * drag-n-drop in QListWidget
// * directory tree in QTreeWidget
// * combo Widget

QWidget* get_list_widget(QWidget* parent){

    QWidget* show_list_widget = new QWidget(parent);
    QHBoxLayout* list_layout = new QHBoxLayout;
    QListWidget* lst = new QListWidget;
    
    // TODO: manage Drag&Drop
    //lst->setDragEnabled(true);
    ToggleListStyles* toggle_list = new ToggleListStyles("List styles", show_list_widget);
    toggle_list->set_buddy_list(lst);
    toggle_list->style_changed_slot();
    list_layout->addWidget(toggle_list);
    list_layout->addWidget(lst);
    show_list_widget->setLayout(list_layout);

    return show_list_widget;
}

QTreeWidget* get_tree_widget(QWidget* parent){

    QTreeWidget* tree = new QTreeWidget;
    QStringList columns;
    columns << "Directories" << "Used space";
    tree->setHeaderLabels(columns);

    // Allow sorting by click on the header
    tree->setSortingEnabled(true);

    // root directory
    QTreeWidgetItem* root_item = new QTreeWidgetItem(tree);
    root_item->setText(0, "Local Disk C:");
    root_item->setIcon(0, QPixmap("drive_win7.png"));

    // directories (item could be checkable!)
    for (size_t i = 0; i < 10; ++i)
    {
        QTreeWidgetItem* sub_item = new QTreeWidgetItem(root_item);
        
        sub_item->setText(0, QString("Directory 0%1").arg(QString::number(i)));
        sub_item->setIcon(0, QPixmap("directory_win7.png"));
        sub_item->setText(1, "1 MB");
    }
    tree->sortItems(1, Qt::AscendingOrder);
    tree->setItemExpanded(root_item, true);

    return tree;
}

QTableWidget* get_table_widget(QWidget* parent){
    QStringList columns;
    QStringList rows;
    columns << "First" << "Second" << "Third";
    rows << "1" << "2" << "3" << "4";
    QTableWidget* table = new QTableWidget(rows.count(), columns.count(), parent);
    table->setHorizontalHeaderLabels(columns);
    table->setVerticalHeaderLabels(rows);

    for (int i = 0; i < rows.count(); ++i)
    {
        for (int j = 0; j < columns.count(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(QString("Item %1.%2").arg(i).arg(j));
            table->setItem(i, j, item);
        }
    }

    return table;
}

QWidget* get_combo_widget(){
    return new QWidget;
}

// Switch between main window types
#if 1
#define MAIN_WIDGET QTabWidget
#define ADD_ITEM wnd.addTab
#else 
#define MAIN_WIDGET QToolBox
#define ADD_ITEM wnd.addItem
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MAIN_WIDGET wnd;
    ADD_ITEM(get_list_widget(&wnd), "QListWidget");
    ADD_ITEM(get_tree_widget(&wnd), "QTreeWidget");
    ADD_ITEM(get_table_widget(&wnd), "QTableWidget");

    wnd.show();
    return app.exec();
}
