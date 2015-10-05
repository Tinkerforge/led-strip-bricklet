Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID
    Const NUM_LEDS As Integer = 16

    Dim rIndex As Integer = 0
    Dim r() As Byte = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    Dim g() As Byte = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    Dim b() As Byte = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

    ' Use frame rendered callback to move the active LED every frame
    Sub FrameRenderedCB(ByVal sender As BrickletLEDStrip, ByVal length As Integer)
        b(rIndex) = 0
        If rIndex = NUM_LEDS-1 Then
            rIndex = 0
        Else
            rIndex = rIndex + 1
        End If
        b(rIndex) = 255

        ' Set new data for next render cycle
        sender.SetRGBValues(0, NUM_LEDS, r, g, b)
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim ls As New BrickletLEDStrip(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Set frame duration to 50ms (20 frames per second)
        ls.SetFrameDuration(50)

        ' Register frame rendered callback to subroutine FrameRenderedCB
        AddHandler ls.FrameRendered, AddressOf FrameRenderedCB

        ' Set initial rgb values to get started
        ls.SetRGBValues(0, NUM_LEDS, r, g, b)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
