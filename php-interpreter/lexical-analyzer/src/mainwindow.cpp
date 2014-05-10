#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QString>
#include <QFile>

#include "LexicalAnalyzer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mp_result_table->setColumnWidth(0, 154);
    ui->mp_result_table->setColumnWidth(1, 200);
    ui->mp_result_table->setColumnWidth(2, 70);
    ui->mp_result_table->setColumnWidth(3, 70);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_record(const Token& i_token)
{
    ui->mp_result_table->setRowCount(ui->mp_result_table->rowCount() + 1);

    ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 0, new QTableWidgetItem(TokenToString(i_token.m_token_type)));
    ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 1, new QTableWidgetItem(i_token.m_lexem));
    ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 3, new QTableWidgetItem(QString::number(i_token.m_row)));
    ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 2, new QTableWidgetItem(QString::number(i_token.m_column)));
}

void MainWindow::load_source()
{
    QFile source_file(ui->mp_sourcefile_path->text());
    if(!source_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->mp_file_status->setText("Bad file name!");
        ui->mp_sourcefile_path->setText(QApplication::applicationDirPath());
        return;
    }

    ui->mp_file_status->setText("");
    ui->mp_source->setPlainText("");

    QString source_file_text("");
    while (!source_file.atEnd())
    {
        source_file_text += source_file.readLine();
    }

    ui->mp_source->setPlainText(source_file_text);
}

void MainWindow::on_mp_sourcefile_path_returnPressed()
{
    load_source();
}

void MainWindow::on_mp_load_button_clicked()
{
    load_source();
}

void MainWindow::on_mp_analize_button_clicked()
{
    LexicalAnalyzer anal(ui->mp_source->toPlainText());
    for (Token token; anal.nextToken(token);)
    {
      add_record(token);
      return;
    }
}
