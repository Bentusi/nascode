/**
 * @file test_pou_serialization.cpp
 * @brief POU序列化功能测试示例
 * 
 * 演示如何：
 * 1. 创建POU对象
 * 2. 添加变量
 * 3. 序列化为JSON
 * 4. 生成ST代码
 */

#include "../src/models/POU.h"
#include "../src/models/Project.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

using namespace nascode::models;

int main()
{
    // 1. 创建一个Program
    auto mainProg = std::make_shared<Program>("MainProgram");
    mainProg->setComment("主程序 - 控制系统入口");
    
    // 2. 添加输入变量
    Variable startButton;
    startButton.name = "StartButton";
    startButton.dataType = "BOOL";
    startButton.initialValue = "FALSE";
    startButton.comment = "启动按钮";
    startButton.section = Variable::VAR_INPUT;
    mainProg->addVariable(startButton);
    
    Variable stopButton;
    stopButton.name = "StopButton";
    stopButton.dataType = "BOOL";
    stopButton.initialValue = "FALSE";
    stopButton.comment = "停止按钮";
    stopButton.section = Variable::VAR_INPUT;
    mainProg->addVariable(stopButton);
    
    // 3. 添加输出变量
    Variable motorRunning;
    motorRunning.name = "MotorRunning";
    motorRunning.dataType = "BOOL";
    motorRunning.initialValue = "FALSE";
    motorRunning.comment = "电机运行状态";
    motorRunning.section = Variable::VAR_OUTPUT;
    mainProg->addVariable(motorRunning);
    
    // 4. 添加内部变量
    Variable counter;
    counter.name = "Counter";
    counter.dataType = "INT";
    counter.initialValue = "0";
    counter.comment = "计数器";
    counter.section = Variable::VAR;
    mainProg->addVariable(counter);
    
    Variable timer;
    timer.name = "RunTimer";
    timer.dataType = "TIME";
    timer.initialValue = "T#0s";
    timer.comment = "运行时间";
    timer.section = Variable::VAR;
    mainProg->addVariable(timer);
    
    // 5. 设置代码
    QString code = R"(
// 启动逻辑
IF StartButton AND NOT StopButton THEN
    MotorRunning := TRUE;
    Counter := Counter + 1;
END_IF;

// 停止逻辑
IF StopButton THEN
    MotorRunning := FALSE;
END_IF;
)";
    mainProg->setCode(code);
    
    // 6. 生成ST代码并打印
    qDebug() << "=== Generated ST Code ===";
    qDebug() << mainProg->generateSTCode();
    
    // 7. 序列化为JSON
    QVariantMap pouData = mainProg->toVariant();
    QJsonObject jsonObj = QJsonObject::fromVariantMap(pouData);
    QJsonDocument doc(jsonObj);
    
    qDebug() << "\n=== Serialized JSON ===";
    qDebug() << doc.toJson(QJsonDocument::Indented);
    
    // 8. 创建项目并添加POU
    Project project;
    project.setName("TestProject");
    project.setDescription("测试项目 - 演示POU序列化");
    project.addPOU(mainProg);
    
    // 9. 创建一个Function
    auto calcFunction = std::make_shared<Function>("CalculateSpeed");
    calcFunction->setComment("计算速度");
    
    Variable distance;
    distance.name = "Distance";
    distance.dataType = "REAL";
    distance.section = Variable::VAR_INPUT;
    calcFunction->addVariable(distance);
    
    Variable time;
    time.name = "Time";
    time.dataType = "REAL";
    time.section = Variable::VAR_INPUT;
    calcFunction->addVariable(time);
    
    Variable speed;
    speed.name = "Speed";
    speed.dataType = "REAL";
    speed.section = Variable::VAR_OUTPUT;
    calcFunction->addVariable(speed);
    
    calcFunction->setCode("Speed := Distance / Time;");
    project.addPOU(calcFunction);
    
    // 10. 创建一个FunctionBlock
    auto pidBlock = std::make_shared<FunctionBlock>("PID_Controller");
    pidBlock->setComment("PID控制器");
    
    Variable setpoint;
    setpoint.name = "Setpoint";
    setpoint.dataType = "REAL";
    setpoint.section = Variable::VAR_INPUT;
    pidBlock->addVariable(setpoint);
    
    Variable processValue;
    processValue.name = "ProcessValue";
    processValue.dataType = "REAL";
    processValue.section = Variable::VAR_INPUT;
    pidBlock->addVariable(processValue);
    
    Variable output;
    output.name = "Output";
    output.dataType = "REAL";
    output.section = Variable::VAR_OUTPUT;
    pidBlock->addVariable(output);
    
    Variable kp;
    kp.name = "Kp";
    kp.dataType = "REAL";
    kp.initialValue = "1.0";
    kp.comment = "比例增益";
    kp.section = Variable::VAR;
    pidBlock->addVariable(kp);
    
    Variable ki;
    ki.name = "Ki";
    ki.dataType = "REAL";
    ki.initialValue = "0.1";
    ki.comment = "积分增益";
    ki.section = Variable::VAR;
    pidBlock->addVariable(ki);
    
    Variable kd;
    kd.name = "Kd";
    kd.dataType = "REAL";
    kd.initialValue = "0.01";
    kd.comment = "微分增益";
    kd.section = Variable::VAR;
    pidBlock->addVariable(kd);
    
    pidBlock->setCode(R"(
VAR
    Error : REAL;
    Integral : REAL;
    Derivative : REAL;
    LastError : REAL;
END_VAR

Error := Setpoint - ProcessValue;
Integral := Integral + Error;
Derivative := Error - LastError;
Output := Kp * Error + Ki * Integral + Kd * Derivative;
LastError := Error;
)");
    project.addPOU(pidBlock);
    
    // 11. 保存项目到文件
    QString projectPath = "/tmp/test_project.nascode";
    if (project.saveToFile(projectPath)) {
        qDebug() << "\n=== Project saved successfully ===";
        qDebug() << "Project file:" << projectPath;
        qDebug() << "POU count:" << project.getAllPOUs().size();
        
        // 读取文件内容
        QFile file(projectPath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument projectDoc = QJsonDocument::fromJson(file.readAll());
            qDebug() << "\n=== Complete Project JSON ===";
            qDebug() << projectDoc.toJson(QJsonDocument::Indented);
            file.close();
        }
    }
    
    // 12. 测试加载
    Project loadedProject;
    if (loadedProject.loadFromFile(projectPath)) {
        qDebug() << "\n=== Project loaded successfully ===";
        qDebug() << "Loaded project name:" << loadedProject.name();
        qDebug() << "Loaded POU count:" << loadedProject.getAllPOUs().size();
        
        // 验证POU
        auto loadedProg = loadedProject.getPOU("MainProgram");
        if (loadedProg) {
            qDebug() << "\n=== Loaded MainProgram ===";
            qDebug() << "Variables count:" << loadedProg->variables().size();
            qDebug() << "Code preserved:" << (loadedProg->code() == code);
        }
    }
    
    return 0;
}
