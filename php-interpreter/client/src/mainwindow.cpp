#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QString>
#include <QFile>
#include <algorithm>

#include "LA_Aux.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalizer.h"

#include "TokenSource.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
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

  ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 0, new QTableWidgetItem(toString(i_token.m_token_type)));
  ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 1, new QTableWidgetItem(i_token.m_lexem));
  ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 2, new QTableWidgetItem(QString::number(i_token.m_row)));
  ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 3, new QTableWidgetItem(QString::number(i_token.m_column)));
}

void MainWindow::load_source()
{
  QFile source_file(ui->mp_sourcefile_path->text());
  if (!source_file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    ui->mp_file_status->setText("Bad file name! Maybe you should use this filename?");
    ui->mp_sourcefile_path->setText(QApplication::applicationDirPath() + "/Data/source_1.php");
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
  ui->mp_result_table->setRowCount(0);

  QVector<Token> all_tokens;

  // Lexical tab
  LexicalAnalyzer lex(ui->mp_source->toPlainText());
  for (Token token; lex.nextToken(token);)
  {
    add_record(token);
    all_tokens.push_back(token);
  }

  // Syntax tab
  SyntaxAnalyzer syntax = SyntaxAnalyzer(TokenSource(all_tokens));

  for (ProductionResult prod = syntax.readProduction(); !prod.isEmpty(); prod = syntax.readProduction())
  {
    std::for_each(std::begin(prod), std::end(prod), [&](const Production& i_sub_prod) {
      ui->mp_syntax_output->appendPlainText(i_sub_prod);
    });
  }
}
