import sys
import time
import BL1_signature_show
from BL1_encryption import *
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
            BL1_signature_show.MyApp.dialog_open(self,push)
            
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
            BL1_signature_show.MyApp.get_key(self,push)

class Main(QDialog):
    x = 0
    
    def __init__(self):
        super().__init__()
        self.init_ui()

    def init_ui(self):        
        
        self.setWindowTitle("1st BootLoader image generator")
        layout = QVBoxLayout()
        
        root_path = "C:/"
        self.model_file_system = QFileSystemModel()
        self.model_file_system.setRootPath(root_path)
        self.model_file_system.setReadOnly(False)

        #input = Forensic start point -> can be changed by a user
        start_index = "C:/Users"
        self.tree_view = QTreeView()
        self.tree_view.setModel(self.model_file_system)
        self.tree_view.setRootIndex(self.model_file_system.index(start_index))
        self.tree_view.doubleClicked.connect(lambda index : self.item_double_clicked(index))

        self.tree_view.setDragEnabled(True)
        self.tree_view.setColumnWidth(0,300)
        
        self.lineedit1 = QLineEdit1()
        self.lineedit2 = QLineEdit2()
        
        self.btn2 = QPushButton('Make final image',self)
        self.btn2.setCheckable(True)
        self.btn2.toggle()
        self.btn2.clicked.connect(self.buttonclicked)

        layout.addWidget(self.tree_view)
        layout.addWidget(self.lineedit1)        
        layout.addWidget(self.lineedit2)
        layout.addWidget(self.btn2)
        
        self.setLayout(layout)
        self.resize(1200, 800)
        self.show()
    
    def buttonclicked(self):
        BL1_signature_show.MyApp.encrypt(BL1_signature_show.MyApp)
        time.sleep(1)
        BL1_signature_show.MyApp.second_window(BL1_signature_show.MyApp)

    
    #get the file path when it double_clicked
    def item_double_clicked(self, index):
        print(self.model_file_system.filePath(index))

if __name__ == '__main__':
    app = QApplication(sys.argv)
    main = Main()
    sys.exit(app.exec_())
