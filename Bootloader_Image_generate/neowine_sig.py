import sys
import signature_show
from encryption import *
from PyQt5.QtWidgets import *
#make tree and visualize
class QTreeView(QTreeView):
    def __init__(self):
        super(QTreeView, self).__init__()

    def edit(self, index, trigger, event):
        return False
#make a blank that can drag and drop the file
#the file that can be encoded by UTF-8 only be read by dialog_open
class QLineEdit1(QLineEdit): 
    def __init__(self):
        super(QLineEdit, self).__init__()
        self.pathLabel = QLabel(self)
        self.setAcceptDrops(True)

    def dragEnterEvent(self, event):
        data = event.mimeData()
        urls = data.urls()
        if (urls and urls[0].scheme() == 'file'):
            event.acceptProposedAction()
    
    # link it to widget, get the data about file
    def dropEvent(self, event):
        data = event.mimeData()
        urls = data.urls()
        if (urls and urls[0].scheme() == 'file'):
            filepath = str(urls[0].path())[1:]
            self.setText(filepath)
            push = (filepath, 'All Files (*)')
            signature_show.MyApp.dialog_open(self,push)
            
class QLineEdit2(QLineEdit): 
    def __init__(self):
        super(QLineEdit, self).__init__()
        self.pathLabel = QLabel(self)
        self.setAcceptDrops(True)

    def dragEnterEvent(self, event):
        data = event.mimeData()
        urls = data.urls()
        if (urls and urls[0].scheme() == 'file'):
            event.acceptProposedAction()
    
    # link it to widget, get the data about file
    def dropEvent(self, event):
        data = event.mimeData()
        urls = data.urls()
        if (urls and urls[0].scheme() == 'file'):
            filepath = str(urls[0].path())[1:]
            self.setText(filepath)
            push = (filepath, 'All Files (*)')
            signature_show.MyApp.get_key(self,push)

class Main(QDialog):
    x = 0
    
    def __init__(self):
        super().__init__()
        self.init_ui()

    def init_ui(self):        
        
        self.setWindowTitle("BootLoader image generatorpy")
        layout = QVBoxLayout()
        
        root_path = "C:/"
        self.model_file_system = QFileSystemModel()
        self.model_file_system.setRootPath(root_path)
        self.model_file_system.setReadOnly(False)

        #input = Forensic start point -> can be changed by a user
        start_index = "C:/Users/kibong/Desktop"
        self.tree_view = QTreeView()
        self.tree_view.setModel(self.model_file_system)
        self.tree_view.setRootIndex(self.model_file_system.index(start_index))
        self.tree_view.doubleClicked.connect(lambda index : self.item_double_clicked(index))

        self.tree_view.setDragEnabled(True)
        self.tree_view.setColumnWidth(0,300)
        
        self.lineedit1 = QLineEdit1()
        self.lineedit2 = QLineEdit2()
        self.lineedit3 = QLineEdit2()
        self.le = QLineEdit()
        
        self.btn1 = QPushButton('Input Verision', self)
        self.btn1.clicked.connect(self.showDialog)
        
        self.btn2 = QPushButton('Make Signature',self)
        self.btn2.setCheckable(True)
        self.btn2.toggle()
        self.btn2.clicked.connect(self.buttonclicked)

        layout.addWidget(self.tree_view)
        layout.addWidget(self.lineedit1)        
        layout.addWidget(self.lineedit2)
        layout.addWidget(self.lineedit3)
        layout.addWidget(self.le)
        layout.addWidget(self.btn1)
        layout.addWidget(self.btn2)
        
        self.setLayout(layout)
        self.resize(1200, 800)
        self.show()
    
    def buttonclicked(self):
        sig = signature_show.MyApp.encrypt(signature_show.MyApp,Main.x)
        signature_show.MyApp.second_window(signature_show.MyApp,sig)
    
    def showDialog(self):
        text, ok = QInputDialog.getText(self, 'Input Dialog', 'Enter Version:')

        if ok:
            self.le.setText(str(text))
            Main.x = int(text.replace(".","0"))
    
    #get the file path when it double_clicked
    def item_double_clicked(self, index):
        print(self.model_file_system.filePath(index))

if __name__ == '__main__':
    app = QApplication(sys.argv)
    main = Main()
    sys.exit(app.exec_())
