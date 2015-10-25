#include <QApplication>
#include <QTabWidget>
#include <QLayout>
#include <QStringListModel>
#include <QListView>
#include <QTreeView>
#include <QTableView>
#include <QSplitter>
#include <QLineEdit>
#include <QListWidget>

#include <QStandardItemModel>
#include <QDirModel>
#include <QSortFilterProxyModel>

#include "simple_delegate.h"
#include "int_list_model.h"
#include "table_model.h"

// TODO: nested tables - is it possible insert to every cell?
// TODO: SQL Model
// TODO: Understand what is roles
// TODO: make sample explorer style similar to Windows explorer

// * model is responsible for data storage, access and write
// (QDirModel - file system, QStandardItemMode - simple in-memory, QProxyModex - sorting&filtering etc)
// * view - base class for representation

// * Selection model allows defining selection behavior in the View
// * standard Selection model is QItemSelectionModel
// * Delegate is responsible for the single View element draw and edit, 
// also read from Model and write back to Model

QWidget* get_selection_model_example(QWidget* parent){

    // Selection model allows defining selection behavior in the View
    // standard Selection model is QItemSelectionModel
    QWidget* selection_widget = new QWidget(parent);

    // Simple list-based model
    QStringListModel* model = new QStringListModel(parent);
    QStringList l;
    l << "Item 1" << "Item 2" << "Item 3";
    model->setStringList(l);

    // Create three views
    QListView* list_view = new QListView;
    QTreeView* tree_view = new QTreeView;
    QTableView* table_view = new QTableView;
    list_view->setModel(model);
    tree_view->setModel(model);
    table_view->setModel(model);

    // Special model for selection
    QItemSelectionModel* selection = new QItemSelectionModel(model);

    // Set the same selection model - so selected element will be selected in every view
    list_view->setSelectionModel(selection);
    tree_view->setSelectionModel(selection);
    table_view->setSelectionModel(selection);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(list_view);
    layout->addWidget(tree_view);
    layout->addWidget(table_view);
    selection_widget->setLayout(layout);

    return selection_widget;
}

QWidget* get_simple_delegate_example(QWidget* parent){

    // Simple list-based model
    QStringListModel* model = new QStringListModel(parent);
    QStringList l;
    l << "Item 1" << "Item 2" << "Item 3";
    model->setStringList(l);

    // View for the model
    QListView* lv = new QListView;
    lv->setModel(model);

    // Set our delegate
    SimpleDelegate* sd = new SimpleDelegate(lv);
    lv->setItemDelegate(sd);
    
    // Make the QListView react to mouse hover
    // 
    lv->viewport()->setAttribute(Qt::WA_Hover);
    return lv;
}

QWidget* get_index_example(QWidget* parent){
    
    QTreeView* tree_view = new QTreeView(parent);

    static const int rows_total = 5;
    static const int columns_total = 3;

    // Simple in-memory model (table 5x3)
    QStandardItemModel* model = new QStandardItemModel(rows_total, columns_total, parent);

    // 4 nested cycles - every cell of model contain a table
    for (int top_row = 0; top_row < rows_total; ++top_row)
    {
        for (int top_column = 0; top_column < columns_total; ++top_column)
        {
            QModelIndex index = model->index(top_row, top_column);
            model->setData(index, QString("item %1-%2").arg(top_row).arg(top_column));

            // Create nested table
            int nested_rows_total = 4;
            int nested_columns_total = 3;

            // TODO: is it possible insert to every cell?
            model->insertRows(0, nested_rows_total, index);
            model->insertColumns(0, nested_columns_total, index);
            for (int nested_row = 0; nested_row < nested_rows_total; ++nested_row)
            {
                for (int nested_column = 0; nested_column < nested_columns_total; ++nested_column)
                {
                    QString internal_value = QString("%1.%2").arg(nested_row).arg(nested_column);
                    model->setData(model->index(nested_row, nested_column, index), internal_value);
                }
            }
        }
    }

    tree_view->setModel(model);

    return tree_view;
}

QWidget* get_explorer_example(QWidget* parent){

    // TODO: make style similar to Windows explorer
    QSplitter* spl = new QSplitter(Qt::Horizontal, parent);
    QDirModel* model = new QDirModel(spl);

    QTreeView* tree_view = new QTreeView;
    QTableView* table_view = new QTableView;
    tree_view->setModel(model);
    table_view->setModel(model);

    // Click tree - select table row
    QObject::connect(tree_view, &QTreeView::clicked, table_view, &QTableView::setRootIndex);

    // Click table row - select tree element
    QObject::connect(table_view, &QTableView::activated, tree_view, &QTreeView::setCurrentIndex);

    // Entering directory in the table view
    QObject::connect(table_view, &QTableView::activated, table_view, &QTableView::setRootIndex);

    spl->addWidget(tree_view);
    spl->addWidget(table_view);

    return spl;
}

QWidget* get_roles_example(QWidget* parent){
    
    QListView* list_view = new QListView(parent);

    QStringList l;
    QStringList icons;
    l << "Control" << "Directory" << "Drive";
    icons << "control_win7.png" << "directory_win7.png" << "drive_win7.png";

    // Simple in-memory table model 3x1
    QStandardItemModel* model = new QStandardItemModel(3, 1, parent);
    for (int i = 0; i < model->rowCount(); ++i)
    {
        QModelIndex index = model->index(i, 0);
        model->setData(index, l.at(i), Qt::DisplayRole);
        model->setData(index, QIcon(icons.at(i)), Qt::DecorationRole);
        model->setData(index, QString("%1 Icon for Windows 7").arg(l.at(i)), Qt::ToolTipRole);
    }
    
    // make icons available
    list_view->setViewMode(QListView::IconMode);
    list_view->setModel(model);

    return list_view;
}


QWidget* get_user_model_example1(QWidget* parent){

    // Try our own model based to integer list
    QSplitter* spl = new QSplitter(Qt::Horizontal, parent);
    IntListModel* model = new IntListModel(spl);

    QList<int> l;
    l << 123 << 456 << 789;
    model->setIntList(l);


    QTreeView* tree_view = new QTreeView;
    QTableView* table_view = new QTableView;
    tree_view->setModel(model);
    table_view->setModel(model);

    spl->addWidget(tree_view);
    spl->addWidget(table_view);

    return spl;
}

QWidget* get_user_model_example2(QWidget* parent)
{
    // Try our own model based on abstract table
    QTableView* table_view = new QTableView(parent);
    TableModel* model = new TableModel(200, 200, table_view);
    table_view->setModel(model);
    return table_view;
}

QWidget* get_proxy_model_example(QWidget* parent)
{
    QWidget* show_proxy_widget = new QWidget(parent);

    // base model with data
    QStringListModel* model = new QStringListModel(show_proxy_widget); 
    model->setStringList(QStringList() 
        << "AAABBB"
        << "BBAA"
        << "ABAB"
        << "AAA" 
        << "BBBA"
        );

    // proxy model without data, for filtering
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel(show_proxy_widget);
    proxy->setSourceModel(model);

    // Wildcard, RegExp, sorting could be set,
    // Here we can set wildcard from LineEdit.

    // Create views
    QListView* view_with_data = new QListView;
    QListView* view_filtered = new QListView;

    view_with_data->setModel(model);
    view_filtered->setModel(proxy);

    // Layout Setup
    QLineEdit* wildcard = new QLineEdit;

    QVBoxLayout* filtered_layout = new QVBoxLayout;
    filtered_layout->addWidget(wildcard);
    filtered_layout->addWidget(view_filtered);

    QObject::connect(wildcard, &QLineEdit::textChanged, proxy, &QSortFilterProxyModel::setFilterWildcard);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(view_with_data);
    layout->addLayout(filtered_layout);

    show_proxy_widget->setLayout(layout);

    return show_proxy_widget;
}

QWidget* get_widget_data_sharing(QWidget* parent)
{
    QWidget* show_sharing_widget = new QWidget(parent);

    QStringList l;
    l << "Item 1" << "Item 2" << "Item 3";

    // List really contains data
    QListWidget* list_with_data = new QListWidget;
    list_with_data->insertItems(0, l);

    // Use previous list as a model
    QListView* list_share_data = new QListView;
    list_share_data->setModel(list_with_data->model());

    // set up layout
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(list_with_data);
    layout->addWidget(list_share_data);
    show_sharing_widget->setLayout(layout);

    return show_sharing_widget;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QTabWidget wnd;
    wnd.addTab(get_selection_model_example(&wnd), "Selection Model");
    wnd.addTab(get_simple_delegate_example(&wnd), "Simple Delegate");
    wnd.addTab(get_index_example(&wnd), "Index");
    wnd.addTab(get_explorer_example(&wnd), "Explorer");
    wnd.addTab(get_roles_example(&wnd), "Roles");
    wnd.addTab(get_user_model_example1(&wnd), "User model 1");
    wnd.addTab(get_user_model_example2(&wnd), "User model 2");
    wnd.addTab(get_proxy_model_example(&wnd), "Proxy");
    wnd.addTab(get_widget_data_sharing(&wnd), "Share data");

    wnd.show();

    return app.exec();
}

