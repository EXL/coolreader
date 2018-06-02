#include "notifymessagebox.h"

#include <QTimer>
#include <QLabel>
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QCoreApplication>
#include <QPropertyAnimation>

#include <QDebug>

namespace
{
    const QColor FILL_COLOR(66, 68, 70);
    const QColor TEXT_COLOR(188, 190, 191);

    const QFont FONT("Segoe UI", 16);
    const int DISPLAY_TIME = 1500;
}

NotifyMessageBox::NotifyMessageBox(const QString& text, QWidget* parent)
    : NotifyMessageBox(text, FONT, DISPLAY_TIME, parent)
{}

NotifyMessageBox::NotifyMessageBox(const QString& text, const QFont& font, int milliseconds, QWidget* parent)
    : QWidget(parent)
    , m_label(text)
    , m_opacity(1.0)
    , m_milliseconds(milliseconds)
{
    setFont(font);

    m_label.prepare(QTransform(), font);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    if (parent)
    {
        QPoint offset(m_label.size().width() / 2 + 20, m_label.size().height() / 2 + 20);
        setGeometry(QRect(parent->rect().center() - offset, parent->rect().center() + offset));
    }
    else
        resize(270, 80);
}

NotifyMessageBox::~NotifyMessageBox()
{}

void NotifyMessageBox::showImmediatly()
{
    show();
    QCoreApplication::processEvents();
}

void NotifyMessageBox::run()
{
    show();
    update();
    QTimer::singleShot(m_milliseconds, this, &NotifyMessageBox::fadeOut);
}

void NotifyMessageBox::fadeOut()
{
    QPropertyAnimation* animation = new QPropertyAnimation(this, "opacity", this);
    connect(animation, &QPropertyAnimation::finished, this, &NotifyMessageBox::deleteLater);

    animation->setDuration(500);
    animation->setStartValue(1.);
    animation->setEndValue(0.);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NotifyMessageBox::setOpacity(qreal opacity)
{
    m_opacity = opacity;
    update();
}

qreal NotifyMessageBox::opacity() const
{
    return m_opacity;
}

void NotifyMessageBox::showMessage(const QString& message, const QFont& font, int milliseconds, QWidget* parent)
{
    (new NotifyMessageBox(message, font, milliseconds, parent))->run();
}

void NotifyMessageBox::showMessage(const QString& message, QWidget* parent)
{
    showMessage(message, FONT, parent);
}

void NotifyMessageBox::showMessage(const QString& message, const QFont& font, QWidget* parent)
{
    showMessage(message, font, DISPLAY_TIME, parent);
}

void NotifyMessageBox::paintEvent(QPaintEvent* event)
{
    QPainter p(this);

    p.setOpacity(m_opacity);
    p.fillRect(event->rect(), FILL_COLOR);
    p.setPen(TEXT_COLOR);
    p.drawRect(event->rect().adjusted(0, 0, -1, -1));
    p.setFont(font());

    QSize halfSize = m_label.size().toSize() / 2;
    p.drawStaticText(rect().center() -= QPoint(halfSize.width(), halfSize.height()), m_label);
}
