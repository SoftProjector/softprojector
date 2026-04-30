# softprojector

> To compile the code, build your project in IDE of your choice, copy the EXE it produces to a folder.
> Run the following command to compile the EXE into runnable project (Make sure you download windeployqt compiler)

```
.\windeployqt.exe --qmldir "<ProjectDirectory>" "<PathtoEXE>"
```

#### A version of SoftProjector which uses HTTP pages for bible/song displays instead of Windows forms.
If you've ever had your mouse or a Windows application open up on the projector during live service, and everyone seeing, then you know the downsides of projecting a computer display. This version of softprojector outputs bible & songs as a webpage, which allows you to add them as scene in OBS. From there you can send the OBS' program mix to Decklink Ouput into your video switcher.

This http implementation opened up different style options of displaying bible text and lyrics. By editing the httpserver.html or running a local file copy, any amount of computers can show and style bible and songs however wanted. Your stream PC may want to show just the primary bible in a lower-thirds style for livestream, and your projection pc output shows parallel.

Furthermore, this now allowed me to add features such as song verse split, where one screen can show the full verse and another shows 2 lines at a time. Perfect for readability on livestream.

Lastly, for us multilinguals, I also added parallel song verse translations which works alongside the verse split feature. This is great because only one person/computer is needed to show different lyrics.

#### To use this, configure your local IP in the settings
Add a browser source in OBS, and then style it using the css setting (example css in :/httpserver.html). Alternatively, copy the .html file to your local machine and edit it as you like.
![tempsnip](https://github.com/SoftProjector/softprojector/assets/23706821/816e8742-cbf1-4967-b50e-5ee511d69a4f)
#### To use the song verse split, check the 'Split Verse Text' checkbox above the song preview list. 
When you go live, the split list will show below the full verses. Simply use your keyboard arrows to navigate.
![267142147-35343950-036b-433e-bbba-28600d464c01](https://github.com/user-attachments/assets/8c24ea6c-ea59-4b38-8b7d-36c22f68e7a1)
#### For parallel verses, add the translation to your song using '='
![vers-tra](https://github.com/user-attachments/assets/5e2d0975-2fb9-4d0a-be0f-17471f30f009)
