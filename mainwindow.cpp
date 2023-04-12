#include "mainwindow.h"
#include "ui_mainwindow.h"

#define steps 6
#define dir 16
#define en 5
#define hm_sen 23

#define stepssm 21
#define dirsm 26
#define ensm 20
#define hm_sensm 24

#define LED_PWM 12
#define LED_BASE 200

#define dataPin 19
#define clockPin 18
#define latchPin 13

#define speed 10
#define REF 50000

static float pb1,pb2,pb3,pb4,pb5,pb6,pb7,pb8;
static float pa1,pa2,pa3,pa4,pa5,pa6,pa7,pa8;
static float a[8];

// int speed = 10;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DEV_Module_Init();
    wiringPiSetup ();
    pinMode (en, OUTPUT) ;
    pinMode (dir, OUTPUT) ;
    pinMode (steps, OUTPUT) ;
    pinMode (hm_sen, INPUT) ;
    digitalWrite(en,HIGH);

    pinMode (ensm, OUTPUT) ;
    pinMode (dirsm, OUTPUT) ;
    pinMode (stepssm, OUTPUT) ;
    pinMode (hm_sensm, INPUT) ;
    digitalWrite(ensm,HIGH);

    sr595Setup (LED_BASE, 8, dataPin, clockPin, latchPin) ;
    pinMode (LED_PWM, PWM_OUTPUT);
    digitalWrite(LED_PWM,1024);
    ADS1263_SetMode(0);
    ADS1263_init_ADC1(ADS1263_400SPS);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ADS1263_SetMode(0);
    ADS1263_init_ADC1(ADS1263_400SPS);
}

void MainWindow::on_pushButton_2_clicked()
{
    extern float pb1,pb2,pb3,pb4,pb5,pb6,pb7,pb8;
    unsigned int samples = 20;
    UDOUBLE ADC[8][samples],ch[8]={0,0,0,0,0,0,0,0};
    for(unsigned int i=0;i<samples;i++)
    {
        for(unsigned int j=0;j<8;j++)
        {
            ADC[j][i]=ADS1263_GetChannalValue(j);
            if((ADC[j][i]>>31) == 1)
            {
                ADC[j][i]=(REF*2 - ADC[j][i]/2147483648.0 * REF);

            }
            else
            {
                ADC[j][i]=(ADC[j][i]/2147483647.0 * REF);

            }
        }
    }
    for (unsigned int i=0;i<samples;i++)
    {
        ch[0]+=ADC[0][i];
        ch[1]+=ADC[1][i];
        ch[2]+=ADC[2][i];
        ch[3]+=ADC[3][i];
        ch[4]+=ADC[4][i];
        ch[5]+=ADC[5][i];
        ch[6]+=ADC[6][i];
        ch[7]+=ADC[7][i];
        if(i==samples-1)
        {
            pb1=ch[0]/samples;
            pb2=ch[1]/samples;
            pb3=ch[2]/samples;
            pb4=ch[3]/samples;
            pb5=ch[4]/samples;
            pb6=ch[5]/samples;
            pb7=ch[6]/samples;
            pb8=ch[7]/samples;
        }
    }

    qDebug()<<pb1<<pb2<<pb3<<pb4<<pb5<<pb6<<pb7<<pb8;
}

void MainWindow::on_pushButton_17_clicked()
{
    extern float pa1,pa2,pa3,pa4,pa5,pa6,pa7,pa8;
    extern float a[8];
    unsigned int samples = 20;
    UDOUBLE ADC[8][samples],ch[8]={0,0,0,0,0,0,0,0};
    for(unsigned int i=0;i<samples;i++)
    {
        for(unsigned int j=0;j<8;j++)
        {
            ADC[j][i]=ADS1263_GetChannalValue(j);
            if((ADC[j][i]>>31) == 1)
            {
                ADC[j][i]=(REF*2 - ADC[j][i]/2147483648.0 * REF);

            }
            else
            {
                ADC[j][i]=(ADC[j][i]/2147483647.0 * REF);

            }
        }
    }
    for (unsigned int i=0;i<samples;i++)
    {
        ch[0]+=ADC[0][i];
        ch[1]+=ADC[1][i];
        ch[2]+=ADC[2][i];
        ch[3]+=ADC[3][i];
        ch[4]+=ADC[4][i];
        ch[5]+=ADC[5][i];
        ch[6]+=ADC[6][i];
        ch[7]+=ADC[7][i];
        if(i==samples-1)
        {
            pa1=ch[0]/samples;
            pa2=ch[1]/samples;
            pa3=ch[2]/samples;
            pa4=ch[3]/samples;
            pa5=ch[4]/samples;
            pa6=ch[5]/samples;
            pa7=ch[6]/samples;
            pa8=ch[7]/samples;
        }
    }
    qDebug()<<pa1<<pa2<<pa3<<pa4<<pa5<<pa6<<pa7<<pa8;
    a[0]=pa1=log10(pb1/pa1);
    a[1]=pa2=log10(pb2/pa2);
    a[2]=pa3=log10(pb3/pa3);
    a[3]=pa4=log10(pb4/pa4);
    a[4]=pa5=log10(pb5/pa5);
    a[5]=pa6=log10(pb6/pa6);
    a[6]=pa7=log10(pb7/pa7);
    a[7]=pa8=log10(pb8/pa8);

    qDebug()<<QString::number(pa1, 'f', 4)<<QString::number(pa2, 'f', 4)<<QString::number(pa3, 'f', 4)<<QString::number(pa4, 'f', 4)
           <<QString::number(pa5, 'f', 4)<<QString::number(pa6, 'f', 4)<<QString::number(pa7, 'f', 4)<<QString::number(pa8, 'f', 4);

}

void MainWindow::on_pushButton_3_clicked()
{
    UWORD i;
#define ChannelNumber 8
#define REF         5.08
    UBYTE ChannelList[ChannelNumber] = {0, 1, 2, 3, 4, 5, 6, 7};    // The channel must be less than 10

    UDOUBLE Value[ChannelNumber] = {0};
    while(1)
    {
        ADS1263_GetAll(ChannelList, Value, ChannelNumber);  // Get ADC1 value
        for(i=0; i<8; i++)
        {
            if((Value[i]>>31) == 1){
                // qDebug()<<"IN" << i <<(REF*2 - Value[i]/2147483648.0 * REF);      //7fffffff + 1

            }
            else
            {
                qDebug()<<"IN" << i <<(Value[i]/2147483647.0 * REF);       //7fffffff

            }
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    digitalWrite(en,LOW);
    digitalWrite(dir,LOW);

    for (int i=0;i<70000;i++)
    {
        if(digitalRead(hm_sen)==0)
        {
            break;
        }
        else
        {
            digitalWrite(steps, HIGH);
            QThread::usleep(speed);
            digitalWrite(steps, LOW);
            QThread::usleep(speed);
        }
    }
    digitalWrite(en,HIGH);
}


void MainWindow::on_pushButton_5_clicked()
{
    digitalWrite(en,LOW);
    digitalWrite(dir,HIGH);

    for (int i=0;i<65000;i++)
    {
        digitalWrite(steps, HIGH);
        QThread::usleep(speed);
        digitalWrite(steps, LOW);
        QThread::usleep(speed);
    }
    digitalWrite(en,HIGH);
}

void MainWindow::on_pushButton_6_clicked()
{
    QString X2=ui->lineEdit->text();
    int intensity =X2.toInt();

    digitalWrite (LED_BASE + 0,LOW) ;
    //  digitalWrite (LED_BASE + 1,LOW) ;
    digitalWrite (LED_BASE + 2,LOW) ;
    digitalWrite (LED_BASE + 3,LOW) ;
    digitalWrite (LED_BASE + 4,LOW) ;
    digitalWrite (LED_BASE + 5,LOW) ;
    digitalWrite (LED_BASE + 6,LOW) ;
    digitalWrite (LED_BASE + 7,LOW) ;
    pwmWrite (LED_PWM, 1024);


    // digitalWrite (LED_BASE + 0,HIGH) ;
    digitalWrite (LED_BASE + 4,HIGH) ;
    //  digitalWrite (LED_BASE + 2,HIGH) ;   (set LED_BASE numbers blue-4 , red-3)
    //    digitalWrite (LED_BASE + 3,HIGH) ;
    //    digitalWrite (LED_BASE + 4,HIGH) ;
    //    digitalWrite (LED_BASE + 5,HIGH) ;
    //    digitalWrite (LED_BASE + 6,HIGH) ;
    //    digitalWrite (LED_BASE + 7,HIGH) ;
    pwmWrite (LED_PWM, intensity);


    digitalWrite (LED_BASE + 1,LOW) ;
    pwmWrite (LED_PWM, intensity);
}

int MainWindow::adc()
{
    //  UWORD i;
#define ChannelNumber 8
#define REF         5.08
    UBYTE ChannelList[ChannelNumber] = {0, 1, 2, 3, 4, 5, 6, 7};    // The channel must be less than 10

    UDOUBLE Value[ChannelNumber] = {0};

    ADS1263_GetAll(ChannelList, Value, ChannelNumber);  // Get ADC1 value
    for(int i=0;i<50;i++)
    {
        qDebug() << ChannelList[0] <<(Value[0]/2147483647.0 * REF) << ChannelList[1] << (Value[1]/2147483647.0 * REF)
                << ChannelList[2] <<(Value[2]/2147483647.0 * REF) << ChannelList[3] << (Value[3]/2147483647.0 * REF)
                << ChannelList[4] <<(Value[4]/2147483647.0 * REF) << ChannelList[5] << (Value[5]/2147483647.0 * REF)
                << ChannelList[6] <<(Value[6]/2147483647.0 * REF) << ChannelList[7] << (Value[7]/2147483647.0 * REF);
    }
}

int MainWindow::average_adc()
{
#define ChannelNumber 8
#define REF         5.08
#define num_of_data 50
#define num 50
    UBYTE ChannelList[ChannelNumber] = {0, 1, 2, 3, 4, 5, 6, 7};    // The channel must be less than 10

    UDOUBLE Value[ChannelNumber] = {0};

    ADS1263_GetAll(ChannelList, Value, ChannelNumber);  // Get ADC1 value
    for(int i=0;i<num_of_data;i++)
    {
        qDebug() << ChannelList[0] <<(((Value[0]/2147483647.0 * REF) * num)/num_of_data) << ChannelList[1] << (((Value[1]/2147483647.0 * REF) * num)/num_of_data)
                << ChannelList[2] <<(((Value[2]/2147483647.0 * REF) * num)/num_of_data) << ChannelList[3] << (((Value[3]/2147483647.0 * REF) * num)/num_of_data)
                << ChannelList[4] <<(((Value[4]/2147483647.0 * REF) * num)/num_of_data) << ChannelList[5] << (((Value[5]/2147483647.0 * REF) * num)/num_of_data)
                << ChannelList[6] <<(((Value[6]/2147483647.0 * REF) * num)/num_of_data) << ChannelList[7] << (((Value[7]/2147483647.0 * REF) * num)/num_of_data);
    }

    int average = ((Value[0]/2147483647.0 * REF) * num)/num_of_data;
    qDebug()<<average;
}

//void MainWindow::delay1(int i, int total)
//{

//    if (i < 90)
//    {
//      delayMicroseconds(100-i);//for stepper motor delay
//      qDebug()<<"c-1" << i;
//    }
//    else if (i > total - 90)
//    {
//      delayMicroseconds(speed++);//for stepper motor delay
//       qDebug()<<"c-2"<<i <<" "<< speed;
//    }
//    else
//    {
//      delayMicroseconds(speed);//for stepper motor delay
//       qDebug()<<"c-3"<<speed;
//    }
//}


void MainWindow::on_pushButton_9_clicked()
{
    firstposi();

    for(int i=0;i<12;i++)
    {
        adc();
        digitalWrite(en,LOW);
        digitalWrite(dir,LOW);
        for(int i=2400;i>0;i--)
        {
            digitalWrite(steps, HIGH);
            QThread::usleep(speed);
            digitalWrite(steps, LOW);
            QThread::usleep(speed);

        }
        QThread::msleep(1000);
        digitalWrite(en,HIGH);
    }
    on_pushButton_4_clicked();
}

void MainWindow::on_pushButton_10_clicked()
{
    digitalWrite (LED_BASE + 0,LOW) ;
    digitalWrite (LED_BASE + 1,LOW) ;
    digitalWrite (LED_BASE + 2,LOW) ;
    digitalWrite (LED_BASE + 3,LOW) ;
    digitalWrite (LED_BASE + 4,LOW) ;
    digitalWrite (LED_BASE + 6,LOW) ;
    digitalWrite (LED_BASE + 7,LOW) ;
    pwmWrite (LED_PWM, 1024);

    on_pushButton_13_clicked();

    on_pushButton_11_clicked();

    firstpos();

    digitalWrite (LED_BASE + 0,LOW) ;
    digitalWrite (LED_BASE + 2,LOW) ;
    digitalWrite (LED_BASE + 3,LOW) ;
    digitalWrite (LED_BASE + 4,LOW) ;
    digitalWrite (LED_BASE + 5,LOW) ;
    digitalWrite (LED_BASE + 6,LOW) ;
    digitalWrite (LED_BASE + 7,LOW) ;
    pwmWrite (LED_PWM, 1024);

    digitalWrite (LED_BASE + 1,HIGH) ;
    pwmWrite (LED_PWM, 0);

    for(int i=0;i<12;i++)
    {
        adc();
        digitalWrite(en,LOW);
        digitalWrite(dir,HIGH);
        for(int i=0;i<2400;i++)
        {
            digitalWrite(steps, HIGH);
            QThread::usleep(speed);
            digitalWrite(steps, LOW);
            QThread::usleep(speed);
        }
        QThread::msleep(0);
        digitalWrite(en,HIGH);
    }
    digitalWrite (LED_BASE + 0,LOW) ;
    digitalWrite (LED_BASE + 1,LOW) ;
    digitalWrite (LED_BASE + 2,LOW) ;
    digitalWrite (LED_BASE + 3,LOW) ;
    digitalWrite (LED_BASE + 4,LOW) ;
    digitalWrite (LED_BASE + 6,LOW) ;
    digitalWrite (LED_BASE + 7,LOW) ;
    pwmWrite (LED_PWM, 1024);

    on_pushButton_13_clicked();
    
    on_pushButton_12_clicked();

    digitalWrite (LED_BASE + 5,HIGH) ;
    pwmWrite (LED_PWM, 0);


    for(int i=0;i<12;i++)
    {
        adc();

        digitalWrite(en,LOW);
        digitalWrite(dir,LOW);
        for(int i=2400;i>0;i--)
        {
            digitalWrite(steps, HIGH);
            QThread::usleep(speed);
            digitalWrite(steps, LOW);
            QThread::usleep(speed);
        }
        QThread::msleep(0);
        digitalWrite(en,HIGH);
    }
    digitalWrite (LED_BASE + 0,LOW) ;
    digitalWrite (LED_BASE + 1,LOW) ;
    digitalWrite (LED_BASE + 2,LOW) ;
    digitalWrite (LED_BASE + 3,LOW) ;
    digitalWrite (LED_BASE + 4,LOW) ;
    digitalWrite (LED_BASE + 6,LOW) ;
    digitalWrite (LED_BASE + 7,LOW) ;
    pwmWrite (LED_PWM, 1024);

    on_pushButton_4_clicked();

}

void MainWindow::firstpos()
{
    digitalWrite(en,LOW);
    digitalWrite(dir,HIGH);
    for(int i=0;i<31200;i++)
    {
        digitalWrite(steps, HIGH);
        QThread::usleep(speed);
        digitalWrite(steps, LOW);
        QThread::usleep(speed);
    }
    QThread::msleep(0);
    digitalWrite(en,HIGH);
}

void MainWindow::firstposi()
{
    digitalWrite(en,LOW);
    digitalWrite(dir,HIGH);
    for (int i=0;i<60400;i++)
    {
        digitalWrite(steps, HIGH);
        QThread::usleep(speed);
        digitalWrite(steps, LOW);
        QThread::usleep(speed);
    }
    QThread::msleep(0);
    digitalWrite(en,HIGH);
}

void MainWindow::on_pushButton_11_clicked()
{
    digitalWrite (LED_BASE + 0,LOW) ;
    digitalWrite (LED_BASE + 1,LOW) ;
    digitalWrite (LED_BASE + 2,LOW) ;
    digitalWrite (LED_BASE + 3,LOW) ;
    digitalWrite (LED_BASE + 4,LOW) ;
    digitalWrite (LED_BASE + 5,LOW) ;
    digitalWrite (LED_BASE + 6,LOW) ;
    digitalWrite (LED_BASE + 7,LOW) ;
    pwmWrite (LED_PWM, 1024);

    on_pushButton_13_clicked();

    QString X2=ui->lineEdit->text();
    int intensity =X2.toInt();

    digitalWrite (LED_BASE + 0,LOW) ;
    digitalWrite (LED_BASE + 1,LOW) ;
    digitalWrite (LED_BASE + 2,LOW) ;
    digitalWrite (LED_BASE + 3,LOW) ;
    digitalWrite (LED_BASE + 4,LOW) ;
    digitalWrite (LED_BASE + 5,LOW) ;
    digitalWrite (LED_BASE + 6,LOW) ;
    digitalWrite (LED_BASE + 7,LOW) ;
    pwmWrite (LED_PWM, 1024);


    digitalWrite (LED_BASE + 4,HIGH) ;
    pwmWrite (LED_PWM, intensity);

}

void MainWindow::on_pushButton_12_clicked()
{
    digitalWrite (LED_BASE + 0,LOW) ;
    digitalWrite (LED_BASE + 1,LOW) ;
    digitalWrite (LED_BASE + 2,LOW) ;
    digitalWrite (LED_BASE + 3,LOW) ;
    digitalWrite (LED_BASE + 4,LOW) ;
    digitalWrite (LED_BASE + 5,LOW) ;
    digitalWrite (LED_BASE + 6,LOW) ;
    digitalWrite (LED_BASE + 7,LOW) ;
    pwmWrite (LED_PWM, 1024);

    digitalWrite(ensm,LOW);
    digitalWrite(dirsm,LOW);
    for (int i=0;i<19500;i++)
    {
        digitalWrite(stepssm, HIGH);
        QThread::usleep(speed);
        digitalWrite(stepssm, LOW);
        QThread::usleep(speed);
    }
    digitalWrite(ensm,HIGH);

    QString X2=ui->lineEdit->text();
    int intensity =X2.toInt();

    digitalWrite (LED_BASE + 0,LOW) ;
    digitalWrite (LED_BASE + 1,LOW) ;
    digitalWrite (LED_BASE + 2,LOW) ;
    // digitalWrite (LED_BASE + 3,LOW) ;
    digitalWrite (LED_BASE + 4,LOW) ;
    digitalWrite (LED_BASE + 5,LOW) ;
    digitalWrite (LED_BASE + 6,LOW) ;
    digitalWrite (LED_BASE + 7,LOW) ;
    pwmWrite (LED_PWM, 1024);


    digitalWrite (LED_BASE + 3,HIGH) ;
    pwmWrite (LED_PWM, intensity);

}

void MainWindow::on_pushButton_13_clicked()
{
    digitalWrite(ensm,LOW);
    digitalWrite(dirsm,HIGH);
    for (int i=0;i<72000;i++)
    {
        if(digitalRead(hm_sensm)==0)
        {
            break;
        }
        else
        {
            digitalWrite(stepssm, HIGH);
            QThread::usleep(speed);
            digitalWrite(stepssm, LOW);
            QThread::usleep(speed);
        }
    }
    digitalWrite(ensm,HIGH);
}

void MainWindow::on_pushButton_7_clicked()
{
    average_adc();
    qDebug()<<average_adc();
}

void MainWindow::on_pushButton_8_clicked()
{
    adc();
    QThread::msleep(1000);
    qDebug()<<adc();

}

void MainWindow::on_pushButton_14_clicked()
{
    digitalWrite(en,LOW);
    digitalWrite(dir,HIGH);
    for (int i=0;i<60400;i++)
    {
        digitalWrite(steps, HIGH);
        QThread::usleep(speed);
        digitalWrite(steps, LOW);
        QThread::usleep(speed);

    }
    // QThread::msleep(1000);
    digitalWrite(en,HIGH);
}

void MainWindow::on_pushButton_15_clicked()
{
    firstpos();
    for(int i=0;i<12;i++)
    {
        qDebug()<<i;
        adc();
        digitalWrite(en,LOW);
        digitalWrite(dir,HIGH);
        if(i!=11)
        {
            for(int i=0;i<2400;i++)
            {
                digitalWrite(steps, HIGH);
                QThread::usleep(speed);
                digitalWrite(steps, LOW);
                QThread::usleep(speed);
            }
        }
        digitalWrite(en,HIGH);
    }
    on_pushButton_4_clicked();

}

void MainWindow::on_pushButton_16_clicked()
{
    int intensity = ui->lineEdit_2->text().toInt();
    digitalWrite(en,LOW);
    digitalWrite(dir,HIGH);

    for (int i=0;i<intensity;i++)
    {
        digitalWrite(steps, HIGH);
        QThread::usleep(speed);
        digitalWrite(steps, LOW);
        QThread::usleep(speed);
    }
    digitalWrite(en,HIGH);


}



void MainWindow::on_pushButton_18_clicked()
{
    int intensity = ui->lineEdit_2->text().toInt();
    digitalWrite(ensm,LOW);
    digitalWrite(dirsm,LOW);
    for (int i=0;i<intensity;i++)
    {
        digitalWrite(stepssm, HIGH);
        QThread::usleep(speed);
        digitalWrite(stepssm, LOW);
        QThread::usleep(speed);
    }
    digitalWrite(ensm,HIGH);

}

void MainWindow::on_pushButton_19_clicked()
{
    digitalWrite(en,LOW);
    digitalWrite(dir,HIGH);

    for (int i=0;i<16000;i++)
    {
        digitalWrite(steps, HIGH);
        QThread::usleep(speed);
        digitalWrite(steps, LOW);
        QThread::usleep(speed);
    }
    digitalWrite(en,HIGH);

    digitalWrite (LED_BASE + 4,HIGH) ;
    pwmWrite (LED_PWM, 0);
    for(int i=0;i<5;i++)
    {
        on_pushButton_2_clicked();
    }


    digitalWrite(en,LOW);
    for (int i=0;i<15300;i++)
    {
        digitalWrite(steps, HIGH);
        QThread::usleep(speed);
        digitalWrite(steps, LOW);
        QThread::usleep(speed);
    }
    digitalWrite(en,HIGH);

    for (int j=0;j<12;j++)
    {
        on_pushButton_17_clicked();
        for(int i=0;i<8;i++)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            item = new QTableWidgetItem();
            ui->tableWidget->setItem(i, j, item);
            item->setText(QString::number(a[i], 'f', 3));
        }
        digitalWrite(en,LOW);
        for (int i=0;i<2400;i++)
        {
            digitalWrite(steps, HIGH);
            QThread::usleep(speed);
            digitalWrite(steps, LOW);
            QThread::usleep(speed);
        }
        digitalWrite(en,HIGH);
    }
    digitalWrite (LED_BASE + 4,LOW) ;
    pwmWrite (LED_PWM, 1024);
    on_pushButton_4_clicked();

}



void MainWindow::on_pushButton_20_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
