import os
from PIL import Image

# Change the below to your directory containing your images
image_dir = 'C:/Users/lucas/Documents/CAR_2023_Vision/images'

# Janky converter code
os.chdir(image_dir)
for file in os.listdir(image_dir): 
    img = Image.open(file)
    file_name, file_ext = os.path.splitext(file)
    img.save(file_name+'.png')
    img.close()
    os.remove(file_name+file_ext)


