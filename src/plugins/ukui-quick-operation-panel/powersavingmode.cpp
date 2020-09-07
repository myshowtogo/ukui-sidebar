#include "powersavingmode.h"

powerSavingMode::powerSavingMode(QWidget *parent) : QWidget(parent)
{
    initMemberVariables();
    initGsettingValue();
    initButtonStatus();
    initLayout();
}

void powerSavingMode::initMemberVariables()
{
    m_pWidgetButton         = new QWidget();
    m_pWidgetButton->setFixedSize(56, 56);
    m_pWidgetButton->setContentsMargins(0, 0, 0, 0);
    m_pVboxButtonLayout     = new QVBoxLayout();
    m_pVboxButtonLayout->setContentsMargins(0, 0, 0, 0);

    m_pPowerModeButton = new QPushButton();
    m_pPowerModeButton->setFixedSize(56, 56);
    m_pPowerModeButton->setIconSize(QSize(32, 32));
    connect(m_pPowerModeButton, &switchButton::clicked, this, &powerSavingMode::PowerSavingButtonClickSlots);

    m_pStyleNormal = new customstyle_switchNormalStatus("ukui-default");
    m_pPowerModeButton->setStyle(m_pStyleNormal);

    m_pStyleOpen = new CustomStyle_SwitchOpenStatus("ukui-default");

    m_pPowerModeLabel = new QLabel(QObject::tr("节能模式"));
    m_pPowerModeLabel->setAlignment(Qt::AlignHCenter);

    m_pVboxLayout = new QVBoxLayout();
    m_pVboxLayout->setContentsMargins(0, 0, 0, 0);
    m_pVboxLayout->setSpacing(0);

    this->setFixedSize(56, 78);
    this->setContentsMargins(0, 0, 0, 0);
    return;
}

void powerSavingMode::initGsettingValue()
{
    /* 链接电源节能模式gsetting连接 */
    if (QGSettings::isSchemaInstalled(KYLIN_POWER_MODE_GSETTING_VALUE))
        m_pSavingModeGsetting = new QGSettings(KYLIN_POWER_MODE_GSETTING_VALUE);
    if (m_pSavingModeGsetting != nullptr) {
        qDebug() << "当前的gsetting的key值" << m_pSavingModeGsetting->keys();
    }

    /* 用于记录当前节能模式开关状态 */
    if (QGSettings::isSchemaInstalled(UKUI_VOLUME_BRIGHTNESS_GSETTING_ID))
        m_pSavingModeButtonStatusGsetting = new QGSettings(UKUI_VOLUME_BRIGHTNESS_GSETTING_ID);
    if (m_pSavingModeButtonStatusGsetting != nullptr) {
        qDebug() << "当前的gsetting的key值" << m_pSavingModeButtonStatusGsetting->keys();
    }
    return;
}

void powerSavingMode::initButtonStatus()
{
    m_bModelStatus = m_pSavingModeButtonStatusGsetting->get(UKUI_ENERGY_SAVING_MODE_KEY).toBool();
    if (m_bModelStatus) {
        m_pPowerModeButton->setStyle(m_pStyleOpen);
        m_pPowerModeButton->setIcon(QIcon::fromTheme(KYLIN_POWER_SAVING_OPEN_NAME, QIcon(KYLIN_POWER_SAVING_OPEN_PATH)));
    } else {
        m_pPowerModeButton->setStyle(m_pStyleNormal);
        m_pPowerModeButton->setIcon(QIcon::fromTheme(KYLIN_POWER_SAVING_NORMAL_NAME, QIcon(KYLIN_POWER_SAVING_NORMAL_PATH)));
    }
    m_pPowerModeButton->update();
    setLabelTextColor();
}

void powerSavingMode::initLayout()
{
    m_pVboxButtonLayout->addWidget(m_pPowerModeButton, 0, Qt::AlignCenter);
    m_pWidgetButton->setLayout(m_pVboxButtonLayout);
    m_pVboxLayout->addWidget(m_pWidgetButton, 0, Qt::AlignCenter);
    m_pVboxLayout->addItem(new QSpacerItem(15, 4));
    m_pVboxLayout->addWidget(m_pPowerModeLabel);
    this->setLayout(m_pVboxLayout);
    return;
}

QColor powerSavingMode::getSystemPaleteColor()
{
    QStyleOption opt;
    opt.init(m_pPowerModeLabel);
    if (m_bModelStatus) {
        return opt.palette.color(QPalette::Highlight);
    } else {
        return opt.palette.color(QPalette::Text);
    }
}

void powerSavingMode::setLabelTextColor()
{
    QPalette palette;
    QColor color = getSystemPaleteColor();
    palette.setBrush(QPalette::WindowText, color);
    m_pPowerModeLabel->setPalette(palette);
    this->update();
}

void powerSavingMode::PowerSavingButtonClickSlots()
{
    m_bModelStatus = m_pSavingModeButtonStatusGsetting->get(UKUI_ENERGY_SAVING_MODE_KEY).toBool();
    if (!m_bModelStatus) {
        m_pPowerModeButton->setStyle(m_pStyleOpen);
        m_pPowerModeButton->setIcon(QIcon::fromTheme(KYLIN_POWER_SAVING_OPEN_NAME, QIcon(KYLIN_POWER_SAVING_OPEN_PATH)));
        m_pSavingModeGsetting->set(KYLIN_POWER_SREEN_BRIGHTNESS_AC_KEY, 20);
        m_pSavingModeGsetting->set(KYLIN_POWER_SLEEP_DISPLAY_AC_KEY, 60);
        m_pSavingModeGsetting->set(KYLIN_POWER_SLEEP_BATTERY_KEY, 60);
        m_bModelStatus = true;
    } else {
        m_pPowerModeButton->setStyle(m_pStyleNormal);
        m_pPowerModeButton->setIcon(QIcon::fromTheme(KYLIN_POWER_SAVING_NORMAL_NAME, QIcon(KYLIN_POWER_SAVING_NORMAL_PATH)));
        m_pSavingModeGsetting->set(KYLIN_POWER_SREEN_BRIGHTNESS_AC_KEY, 99);
        m_pSavingModeGsetting->set(KYLIN_POWER_SLEEP_DISPLAY_AC_KEY, 600);
        m_pSavingModeGsetting->set(KYLIN_POWER_SLEEP_BATTERY_KEY, 600);
        m_bModelStatus = false;
    }
    m_pSavingModeButtonStatusGsetting->set(UKUI_ENERGY_SAVING_MODE_KEY, m_bModelStatus);
    m_pPowerModeButton->update();
    setLabelTextColor();
}