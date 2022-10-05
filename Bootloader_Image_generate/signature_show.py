from cgitb import text
from cmath import nan
from pickle import NONE
import sys
import subprocess
from neowine_sig import *
from encryption import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5 import uic

class MyApp(QDialog):
    str1 = " "
    str2 = " "
    str3 = " "
    

    
    def dialog_open(self,input):
        if input:
            fname = input
        else:        
            fname = QFileDialog.getOpenFileName(self)  
        if fname[0]:
            self.pathLabel.setText(fname[0])
            print('filepath : ', fname[0])
            print('filesort : ', fname[1])
            MyApp.str1 = fname[0].split('/');
        else:
            QMessageBox.about(self, 'warning', '파일을 선택하지 않았습니다')
    
    def get_key(self,input):
        if input:
            fname = input
        else:        
            fname = QFileDialog.getOpenFileName(self)   
        if fname[0]:
            self.pathLabel.setText(fname[0])
            print('filepath : ', fname[0])
            print('filesort : ', fname[1])
            
            f_key = open(fname[0], "r")
            key = RSA.importKey(f_key.read())
            
            if key.has_private():
                MyApp.str2 = fname[0].split('/');
            else :
                MyApp.str3 = fname[0].split('/');
        else:
            QMessageBox.about(self, 'warning', '파일을 선택하지 않았습니다')
            
    def encrypt(self,ver):
        print(ver)
        command = "NeoRSA.exe "
        command = command + "./" + MyApp.str1[-1] +" ./" +  MyApp.str2[-1] +" ./" +  MyApp.str3[-1]
        print(command)
        subprocess.run(command, shell=True)
        
    def second_window(self):
        window_2 = second()
        window_2.exec_()

    def __init__(self):
        super().__init__()
        QToolTip.setFont(QFont('SansSerif', 10))
        self.btn1 = QPushButton('open file', self)
        self.btn1.clicked.connect(self.dialog_open)
        self.pathLabel = QLabel(self)
        layout = QVBoxLayout()
        layout.addWidget(self.btn1)
        layout.addWidget(self.pathLabel)
        self.setLayout(layout)
        self.setWindowTitle('px4_extracted')
        self.setWindowIcon(QIcon('drone.jpg'))
        self.resize(300,150)
        self.initUI()

    def initUI(self):
        self.show()

class second(QDialog):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        self.textEdit = QTextEdit(self)
        layout.addWidget(self.textEdit)
        self.setLayout(layout)
        self.setWindowTitle('data_shown')
        self.setWindowIcon(QIcon('drone.jpg'))
        self.get_data()
        self.resize(600,700)
        self._initUi_()

    def _initUi_(self):
        self.show()

    def get_data(self):
        with open("signature.bin","rb") as f:
            data = f.read()
        self.textEdit.setText(str(data))

if __name__ == '__main__':
   app = QApplication(sys.argv)
   ex = MyApp()
   sys.exit(app.exec_())
