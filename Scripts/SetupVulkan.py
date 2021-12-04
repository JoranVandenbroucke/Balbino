from zipfile import ZipFile

with ZipFile('../3rdParty/Vulkan.zip', 'r') as zipObj:
   # Extract all the contents of zip file in different directory
   zipObj.extractall('../3rdParty/Vulkan')