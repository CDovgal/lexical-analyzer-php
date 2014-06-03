#pragma once

#include <QMainWindow>

#include <Token.h>

class RuleHighligther;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void add_syntax_record(const Token& i_token);
    void add_semantic_record(const Token& i_token, int i_level);

    void on_mp_sourcefile_path_returnPressed();

    void on_mp_load_button_clicked();

    void on_mp_analize_button_clicked();

private:

    void load_source();

    Ui::MainWindow *ui;
  
    RuleHighligther* m_syntax_highlighter;
};
