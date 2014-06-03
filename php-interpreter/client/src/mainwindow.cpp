#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QString>
#include <QFile>
#include <QSyntaxHighlighter>
#include <algorithm>

#include "LA_Aux.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalizer.h"
#include "SemanticAnalyzer.h"

#include "TokenSource.h"

class RuleHighligther : public QSyntaxHighlighter
{
public:
  RuleHighligther(QTextDocument* ip_parent)
    : QSyntaxHighlighter(ip_parent)
  {}
  
  virtual void highlightBlock(const QString &text)
  {
    {
      QTextCharFormat myClassFormat;
      myClassFormat.setFontWeight(QFont::Bold);
      myClassFormat.setForeground(Qt::blue);
      QString pattern = "#[0-9]+";
    
      QRegExp expression(pattern);
      int index = text.indexOf(expression);
      while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
      }
    }
    
    {
      QTextCharFormat myClassFormat;
      myClassFormat.setFontWeight(QFont::Bold);
      myClassFormat.setForeground(Qt::red);
      QString pattern = "UNEXPECTED ERROR";
      
      QRegExp expression(pattern);
      int index = text.indexOf(expression);
      while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
      }
    }
    
  }
};


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_syntax_highlighter(nullptr)
{
  ui->setupUi(this);
  ui->mp_result_table->setColumnWidth(0, 154);
  ui->mp_result_table->setColumnWidth(1, 200);
  ui->mp_result_table->setColumnWidth(2, 70);
  ui->mp_result_table->setColumnWidth(3, 70);

  ui->mp_semantic_result->setColumnWidth(0, 200);
  ui->mp_semantic_result->setColumnWidth(1, 200);

  showMaximized();
  ui->mp_syntax_output->setTabStopWidth(40);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::add_syntax_record(const Token& i_token)
{
  ui->mp_result_table->setRowCount(ui->mp_result_table->rowCount() + 1);

  ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 0, new QTableWidgetItem(toString(i_token.m_token_type)));
  ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 1, new QTableWidgetItem(i_token.m_lexem));
  ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 2, new QTableWidgetItem(QString::number(i_token.m_row)));
  ui->mp_result_table->setItem(ui->mp_result_table->rowCount() - 1, 3, new QTableWidgetItem(QString::number(i_token.m_column)));
}

void MainWindow::add_semantic_record(const Token& i_token, int i_level)
{
  ui->mp_semantic_result->setRowCount(ui->mp_semantic_result->rowCount() + 1);

  ui->mp_semantic_result->setItem(ui->mp_semantic_result->rowCount() - 1, 0, new QTableWidgetItem(i_token.m_lexem));

  QString init_type = "Unknown";
  if (i_token.m_lexem[0] == '\'' || 
      i_token.m_lexem[0] == '"')
      init_type = "Const string expression";
  else if (i_token.m_lexem.indexOf('.') != -1)
    init_type = "Const double expression";
  else if (i_token.m_lexem[0].isDigit())
    init_type = "Const integer expression";
  else
    init_type = "Const boolean expression";

  ui->mp_semantic_result->setItem(ui->mp_semantic_result->rowCount() - 1, 1, new QTableWidgetItem(init_type));
  ui->mp_semantic_result->setItem(ui->mp_semantic_result->rowCount() - 1, 2, new QTableWidgetItem(QString::number(i_level)));
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
  ui->mp_syntax_output->clear();
  ui->mp_semantic_result->setRowCount(0);
  ui->mp_output_code_generation->clear();

  QVector<Token> all_tokens;

  // Lexical tab
  LexicalAnalyzer lex(ui->mp_source->toPlainText());
  for (Token token; lex.nextToken(token);)
  {
    add_syntax_record(token);
    all_tokens.push_back(token);
  }

  SyntaxAnalyzer syntax = SyntaxAnalyzer(TokenSource(all_tokens));
  ProductionResult prod = syntax.readProduction();
  std::for_each(std::begin(prod), std::end(prod), [&](const Production& i_sub_prod) {
    ui->mp_syntax_output->appendPlainText(i_sub_prod);
  });

  if (nullptr != m_syntax_highlighter)
      delete m_syntax_highlighter;
  
  m_syntax_highlighter = new RuleHighligther(ui->mp_syntax_output->document());

  SemanticAnalysis semantic = SemanticAnalysis(TokenSource(all_tokens));
  auto semantic_result = semantic.result();
  std::for_each(std::begin(semantic_result), std::end(semantic_result), [&](const SemanticResultEntity& i_result) {
    add_semantic_record(i_result.first, i_result.second);
  });
  
  // to be continued...
}
