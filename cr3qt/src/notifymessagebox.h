#pragma once

#include <QWidget>
#include <QStaticText>

class QLabel;

class NotifyMessageBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    explicit NotifyMessageBox(const QString& text, QWidget* parent = 0);
    explicit NotifyMessageBox(const QString& text, const QFont& font, int milliseconds, QWidget* parent = 0);
    virtual ~NotifyMessageBox();

public:
    void showImmediatly();
    void run();

    void setOpacity(qreal opacity);
    qreal opacity() const;

public slots:
    void fadeOut();

public:
    static void showMessage(const QString& message, QWidget* parent);
    static void showMessage(const QString& message, const QFont& font, QWidget* parent);
    static void showMessage(const QString& message, const QFont& font, int milliseconds, QWidget* parent);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QStaticText m_label;
    qreal m_opacity;
    int m_milliseconds;
    QSize first_size;
    bool first = false;
};
