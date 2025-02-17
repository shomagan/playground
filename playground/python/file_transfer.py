#home/kamil/workspace/VGTT_ICM/file_transfer.py
import paramiko
from scp import SCPClient
import re
import logging
import os

def createSSHClient(server, port, user, password):
   client = paramiko.SSHClient()
   client.load_system_host_keys()
   client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
   client.connect(server, port, user, password)
   client.get_transport().window_size = 3 * 1024 * 1024
   return client

logging.basicConfig(level=logging.INFO)

user = 'user'
ssh_password = 'pass'
ssh = createSSHClient('10.10.10.10', 22, user, ssh_password)
source_path = "folder"

with SCPClient(ssh.get_transport()) as scp:
   for (dirpath, dirnames, filenames) in os.walk(source_path):
      for file_name in filenames:
         PATTERN = re.compile(r'_\d.img')
         if PATTERN.match(file_name):
            logging.info(f"skip file: {file_name} \n")
            continue
         PATTERN = re.compile(r'img_\d.sparse')
         if PATTERN.match(file_name):
            logging.info(f"skip file: {file_name} \n")
            continue
         logging.info(f"copy file: {file_name} \n")
         scp.put(source_path + file_name, remote_path=f'/home/{user}/')
      break #break after first level
   for dirpath, dirnames, files in os.walk(source_path):
      for dir_name in dirnames: #all else copy as it is
         logging.info(f"copy folder: {dir_name} \n")
         scp.put(source_path + dir_name, recursive=True, remote_path=f'/home/{user}/')
