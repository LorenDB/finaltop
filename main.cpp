#include <final/final.h>

class MainDialog : public finalcut::FDialog
{
public:
    MainDialog(finalcut::FApplication *app)
        : finalcut::FDialog(app)
    {
        setText("finaltop");
        setGeometry({4, 2}, {17, 3});

        m_about = new finalcut::FLabel("finaltop v0.0.1", this);
        m_about->setGeometry({3, 2}, {17, 1});
    }

private:
    finalcut::FLabel *m_about;
};

int main(int argc, char *argv[])
{
    finalcut::FApplication app(argc, argv);

    MainDialog mainDialog(&app);
    mainDialog.show();
    finalcut::FWidget::setMainWidget(&mainDialog);

    return app.exec();
}
