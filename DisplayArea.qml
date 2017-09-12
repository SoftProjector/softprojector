/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2017  Vladislav Kobzar
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

import QtQuick 2.2
import QtMultimedia 5.0

Rectangle {
    id: dispArea
    visible: true
    color: "#000000"
    anchors.fill: parent

    property int mx: 0
    property int my: 0
    property int mTox: 0
    property int mToy: 0

    property  int rotation1:0
    property  int rotation2:0

    property int tranTime: 500

    // Display Conrol signals
    signal exitClicked()
    signal nextClicked()
    signal prevClicked()

    // Video signals
    signal positionChanged(int position)
    signal durationChanged(int duration)
    signal playbackStateChanged(int state)

    MediaPlayer
    {
        id: player
        objectName: "player"
        autoPlay: false
        onSourceChanged: console.debug(player.source)
        onPositionChanged:
        {
            dispArea.positionChanged(player.position)
        }
        onDurationChanged:
        {
            dispArea.durationChanged(player.duration)
        }
        onPlaybackStateChanged:
        {
            dispArea.playbackStateChanged(player.playbackState)
        }

    }

    VideoOutput
    {
        id: vidOut
        objectName: "vidOut"
        source:player
        anchors.fill: parent
//        fillMode: VideoOutput.Stretch
    }

    Image
    {
        id: backImage1
        objectName: "backImage1"
//        anchors.fill: parent
        cache: false
//        transform: Rotation
//            {
//            id: rotBack1
//            angle: 0
//            axis {x:0; y:0; z:0}
//            origin {x:backImage1/2; y:backImage1/2}
//        }
    }

    Image
    {
        id: backImage2
        objectName: "backImage2"
//        anchors.fill: parent
        cache: false
//        transform: Rotation
//            {
//            id: rotBack2
//            angle: rotation1
//            axis {x:0; y:0; z:0}
//            origin {x:backImage2/2; y:backImage2/2}
//        }
    }

    Image
    {
        id: textImage1
        objectName: "textImage1"
//        anchors.fill: parent
//        fillMode: Image.Stretch
//        anchors.centerIn: parent
        cache: false
//        transform: Rotation
//            {
//            id: rotText1
//            angle: 0
//            axis {x:0; y:0; z:0}
//            origin {x:textImage1/2; y:textImage1/2}
//        }
    }

    Image
    {
        id: textImage2
        objectName: "textImage2"
//        anchors.fill: parent
//        fillMode: Image.Stretch
//        anchors.centerIn: parent
        cache: false
//        transform: Rotation
//            {
//            id: rotText2
//            angle: rotation2
//            axis {x:0; y:0; z:0}
//            origin {x:textImage2/2; y:textImage2/2}
//        }
    }

    Rectangle
    {
        id: controls
        objectName: "controls"
//        opacity: 0.5
        color: "#00000000"
        height: 128
        width: height*3 +20

        x:20; y:20

        Image
        {
            id: prevBtn
            objectName: "prevBtn"
            source: "qrc:/icons/icons/controlPrev.png"
            width: parent.height; height: parent.height

            MouseArea
            {
                id: maPrev
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged:
                {
                    if(maPrev.containsMouse)
                        prevBtn.source = "qrc:/icons/icons/controlPrevHovered.png"
                    else
                        prevBtn.source= "qrc:/icons/icons/controlPrev.png"
                }
                onPressed:
                {
                    prevBtn.source = "qrc:/icons/icons/controlPrevPressed.png"
                }
                onReleased:
                {
                    prevBtn.source = "qrc:/icons/icons/controlPrevHovered.png"
                }
                onClicked:
                {
                    dispArea.prevClicked()
                }
            }
        }

        Image
        {
            id: nextBtn
            objectName: "nextBtn"
            source: "qrc:/icons/icons/controlNext.png"
            width: prevBtn.width; height: prevBtn.height
            anchors.left: prevBtn.right
            anchors.leftMargin: 10
            anchors.top: prevBtn.top
            opacity: prevBtn.opacity
            MouseArea
            {
                id: maNext
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged:
                {
                    if(maNext.containsMouse)
                        nextBtn.source = "qrc:/icons/icons/controlNextHovered.png"
                    else
                        nextBtn.source= "qrc:/icons/icons/controlNext.png"
                }
                onPressed:
                {
                    nextBtn.source = "qrc:/icons/icons/controlNextPressed.png"
                }
                onReleased:
                {
                    nextBtn.source = "qrc:/icons/icons/controlNextHovered.png"
                }
                onClicked:
                {
                    dispArea.nextClicked()
                }
            }
        }

        Image
        {
            id: exitBtn
            objectName: "exitBtn"
            source: "qrc:/icons/icons/controlExit.png"
            width: prevBtn.width; height: prevBtn.height
            anchors.left: nextBtn.right
            anchors.leftMargin: 10
            anchors.top: prevBtn.top
            opacity: prevBtn.opacity
            MouseArea
            {
                id: maExit
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged:
                {
                    if(maExit.containsMouse)
                        exitBtn.source = "qrc:/icons/icons/controlExitHovered.png"
                    else
                        exitBtn.source= "qrc:/icons/icons/controlExit.png"
                }
                onPressed:
                {
                    exitBtn.source = "qrc:/icons/icons/controlExitPressed.png"
                }
                onReleased:
                {
                    exitBtn.source = "qrc:/icons/icons/controlExitHovered.png"
                }
                onClicked:
                {
                    dispArea.exitClicked()
                }
            }
        }
    }

    SequentialAnimation
    {
        id:seqFade1to2
        running: false
        NumberAnimation { target: textImage1; property: "opacity"; to: 0.0; duration: tranTime/2;}
        NumberAnimation { target: textImage2; property: "opacity"; to: 1.0; duration: tranTime/2;}
    }

    SequentialAnimation
    {
        id:seqFade2to1
        running: false
        NumberAnimation { target: textImage2; property: "opacity"; to: 0.0; duration: tranTime/2;}
        NumberAnimation { target: textImage1; property: "opacity"; to: 1.0; duration: tranTime/2;}
    }

    ParallelAnimation
    {
        id:parFade1to2
        running: false
        NumberAnimation { target: textImage1; property: "opacity"; to: 0.0; duration: tranTime;}
        NumberAnimation { target: textImage2; property: "opacity"; to: 1.0; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:parFade2to1
        running: false
        NumberAnimation { target: textImage1; property: "opacity"; to: 1.0; duration: tranTime;}
        NumberAnimation { target: textImage2; property: "opacity"; to: 0.0; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:parBackFade1to2
        running: false
        NumberAnimation { target: backImage1; property: "opacity"; to: 0.0; duration: tranTime;}
        NumberAnimation { target: backImage2; property: "opacity"; to: 1.0; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:parBackFade2to1
        running: false
        NumberAnimation { target: backImage1; property: "opacity"; to: 1.0; duration: tranTime;}
        NumberAnimation { target: backImage2; property: "opacity"; to: 0.0; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:moveTextX1to2
        running:false
        NumberAnimation { target: textImage1; property: "x"; to: mTox; duration: tranTime;}
        NumberAnimation { target: textImage2; property: "x"; to: mx; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:moveTextX2to1
        running:false
        NumberAnimation { target: textImage1; property: "x"; to: mx; duration: tranTime;}
        NumberAnimation { target: textImage2; property: "x"; to: mTox; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:moveTextY1to2
        running:false
        NumberAnimation { target: textImage1; property: "y"; to: mToy; duration: tranTime;}
        NumberAnimation { target: textImage2; property: "y"; to: my; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:moveTextY2to1
        running:false
        NumberAnimation { target: textImage1; property: "y"; to: my; duration: tranTime;}
        NumberAnimation { target: textImage2; property: "y"; to: mToy; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:moveBackX1to2
        running:false
        NumberAnimation { target: backImage1; property: "x"; to: mTox; duration: tranTime;}
        NumberAnimation { target: backImage2; property: "x"; to: mx; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:moveBackX2to1
        running:false
        NumberAnimation { target: backImage1; property: "x"; to: mx; duration: tranTime;}
        NumberAnimation { target: backImage2; property: "x"; to: mTox; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:moveBackY1to2
        running:false
        NumberAnimation { target: backImage1; property: "y"; to: mToy; duration: tranTime;}
        NumberAnimation { target: backImage2; property: "y"; to: my; duration: tranTime;}
    }

    ParallelAnimation
    {
        id:moveBackY2to1
        running:false
        NumberAnimation { target: backImage1; property: "y"; to: my; duration: tranTime;}
        NumberAnimation { target: backImage2; property: "y"; to: mToy; duration: tranTime;}
    }

    SequentialAnimation
    {
        id:rotate1to2
        running: false
//        NumberAnimation{ target: rotText1; property: "angle"; from:0; to:90; duration: 250}
//        NumberAnimation{ target: rotText2; property: "angle"; from:270; to:360; duration: 250}

        NumberAnimation{ target: dispArea; properties: "rotation1"; from:0; to:90; duration: 1000}
        NumberAnimation{ target: dispArea; properties: "rotation2"; from:270; to:360; duration: 1000}
    }

    SequentialAnimation
    {
        id:rotate2to1
        running: false
//        NumberAnimation{ target: rotText2; property: "angle"; from:0; to:90; duration: 250}
//        NumberAnimation{ target: rotText1; property: "angle"; from:270; to:360; duration: 250}
        NumberAnimation{ target: dispArea; properties: "rotation2"; from:0; to:90; duration: 1000}
        NumberAnimation{ target: dispArea; properties: "rotation1"; from:270; to:360; duration: 1000}
    }

    function stopTransitions()
    {
        seqFade1to2.stop()
        seqFade2to1.stop()
        parFade1to2.stop()
        parFade2to1.stop()
        parBackFade1to2.stop()
        parBackFade2to1.stop()
    }

    function transitionText1to2(tranType)
    {
        if(tranType === 1)
        {
            textImage2.opacity = 0.0
            parFade1to2.start()
        }
        else if(tranType === 2)
        {
            textImage2.opacity = 0.0
            seqFade1to2.start()
        }
        else if(tranType === 3)
        {
            mTox = mx + parent.width
            textImage1.y = my
            textImage1.x = mx
            textImage2.y = my
            textImage2.x = mx - parent.width
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            moveTextX1to2.start()
        }
        else if(tranType === 4)
        {
            mTox = mx - parent.width
            textImage1.y = my
            textImage1.x = mx
            textImage2.y = my
            textImage2.x = mx + parent.width
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            moveTextX1to2.start()
        }
        else if(tranType === 5)
        {
            mToy = my - parent.height
            textImage1.y = my
            textImage1.x = mx
            textImage2.y = my + parent.height
            textImage2.x = mx
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            moveTextY1to2.start()
        }
        else if(tranType === 6)
        {
            mToy = my + parent.height
            textImage1.y = my
            textImage1.x = mx
            textImage2.y = my - parent.height
            textImage2.x = mx
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            moveTextY1to2.start()
        }
        else if(tranType === "rotate")
        {
            rotText1.axis.x = 0
            rotText1.axis.y = 1
            rotText2.axis.x = 0
            rotText2.axis.y = 1
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            rotate1to2.start()
        }
        else
        {
            textImage1.opacity = 0.0
            textImage2.opacity = 1.0
            textImage1.x = parent.x
            textImage1.y = parent.y
            textImage2.x = parent.x
            textImage2.y = parent.y
        }
    }

    function transitionText2to1(tranType)
    {
        if(tranType === 1)
        {
            textImage1.opacity = 0.0
            parFade2to1.start()
        }
        else if(tranType === 2)
        {
            textImage1.opacity = 0.0
            seqFade2to1.start()
        }
        else if(tranType === 3)
        {
            mTox = mx + parent.width
            textImage1.y = my
            textImage1.x = mx - parent.width
            textImage2.y = my
            textImage2.x = mx
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            moveTextX2to1.start()
        }
        else if(tranType === 4)
        {
            mTox = mx - parent.width
            textImage1.y = my
            textImage1.x = mx + parent.width
            textImage2.y = my
            textImage2.x = mx
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            moveTextX2to1.start()
        }
        else if(tranType === 5)
        {
            mToy = my - parent.height
            textImage1.y = my + parent.height
            textImage1.x = mx
            textImage2.y = my
            textImage2.x = mx
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            moveTextY2to1.start()
        }
        else if(tranType === 6)
        {
            mToy = my + parent.height
            textImage1.y = my - parent.height
            textImage1.x = mx
            textImage2.y = my
            textImage2.x = mx
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            moveTextY2to1.start()
        }
        else if(tranType === "rotate")
        {
            rotText1.axis.x = 0
            rotText1.axis.y = 1
            rotText2.axis.x = 0
            rotText2.axis.y = 1
            textImage1.opacity = 1.0
            textImage2.opacity = 1.0
            rotate2to1.start()
        }
        else
        {
            textImage1.opacity = 1.0
            textImage2.opacity = 0.0
            textImage1.x = parent.x
            textImage1.y = parent.y
            textImage2.x = parent.x
            textImage2.y = parent.y
        }
    }

    function transitionBack1to2(tranType)
    {
        if(tranType === 1 || tranType === 2)
        {
            backImage2.opacity = 0.0
            parBackFade1to2.start()
        }
        else if(tranType === 3)
        {
            mTox = mx + parent.width
            backImage1.y = my
            backImage1.x = mx
            backImage2.y = my
            backImage2.x = mx - parent.width
            backImage1.opacity = 1.0
            backImage2.opacity = 1.0
            moveBackX1to2.start()
        }
        else if(tranType === 4)
        {
            mTox = mx - parent.width
            backImage1.y = my
            backImage1.x = mx
            backImage2.y = my
            backImage2.x = mx + parent.width
            backImage1.opacity = 1.0
            backImage2.opacity = 1.0
            moveBackX1to2.start()
        }
        else if(tranType === 5)
        {
            mToy = my - parent.height
            backImage1.y = my
            backImage1.x = mx
            backImage2.y = my + parent.height
            backImage2.x = mx
            backImage1.opacity = 1.0
            backImage2.opacity = 1.0
            moveBackY1to2.start()
        }
        else if(tranType === 6)
        {
            mToy = my + parent.height
            backImage1.y = my
            backImage1.x = mx
            backImage2.y = my - parent.height
            backImage2.x = mx
            backImage1.opacity = 1.0
            backImage2.opacity = 1.0
            moveBackY1to2.start()
        }
        else
        {
            backImage1.opacity = 0.0
            backImage2.opacity = 1.0
            backImage1.x = parent.x
            backImage1.y = parent.y
            backImage2.x = parent.x
            backImage2.y = parent.y
        }
    }

    function transitionBack2to1(tranType)
    {
        if(tranType === 1 || tranType === 2)
        {
            backImage1.opacity = 0.0
            parBackFade2to1.start()
        }
        else if(tranType === 3)
        {
            mTox = mx + parent.width
            backImage1.y = my
            backImage1.x = mx - parent.width
            backImage2.y = my
            backImage2.x = mx
            backImage1.opacity = 1.0
            backImage2.opacity = 1.0
            moveBackX2to1.start()
        }
        else if(tranType === 4)
        {
            mTox = mx - parent.width
            backImage1.y = my
            backImage1.x = mx + parent.width
            backImage2.y = my
            backImage2.x = mx
            backImage1.opacity = 1.0
            backImage2.opacity = 1.0
            moveBackX2to1.start()
        }
        else if(tranType === 5)
        {
            mToy = my - parent.height
            backImage1.y = my + parent.height
            backImage1.x = mx
            backImage2.y = my
            backImage2.x = mx
            backImage1.opacity = 1.0
            backImage2.opacity = 1.0
            moveBackY2to1.start()
        }
        else if(tranType === 6)
        {
            mToy = my + parent.height
            backImage1.y = my - parent.height
            backImage1.x = mx
            backImage2.y = my
            backImage2.x = mx
            backImage1.opacity = 1.0
            backImage2.opacity = 1.0
            moveBackY2to1.start()
        }
        else
        {
            backImage1.opacity = 1.0
            backImage2.opacity = 0.0
            backImage1.x = parent.x
            backImage1.y = parent.y
            backImage2.x = parent.x
            backImage2.y = parent.y
        }
    }

//    function setVideoSource(vidSource)
//    {
//        player.source = vidSource
//    }

    function playVideo()
    {
        if(player.playbackState === MediaPlayer.StoppedState
                || player.playbackState === MediaPlayer.PausedState)
        {
            player.play()
        }

    }

    function stopVideo()
    {
        if(player.playbackState === MediaPlayer.PlayingState
                || player.playbackState === MediaPlayer.PausedState)
        {
            player.stop()
        }
    }

    function setVideoVolume(level)
    {
        player.volume = level
    }

    function setVideoMuted(toMute)
    {
        player.muted = toMute
    }

    function setVideoPosition(position)
    {
        player.seek(position)
    }

    function pauseVideo()
    {
        if(player.playbackState === MediaPlayer.PlayingState)
        {
            player.pause()
        }
    }

    function positionControls(iX,iY,iSize,dOpacity)
    {
        controls.height = iSize;
        controls.opacity = dOpacity;
        controls.x = iX;
        controls.y = iY;
    }

    function setControlsVisible(isVisible)
    {
        //controls.opacity = dOpacity;
        controls.visible = isVisible;
    }
}

