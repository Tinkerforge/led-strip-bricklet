program ExampleCallback;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletLEDStrip;

type
  TExample = class
  private
    ipcon: TIPConnection;
    ledStrip: TBrickletLEDStrip;
    r: {$ifdef FPC}array [0..15] of byte{$else}TArray0To15OfUInt8{$endif};
    g: {$ifdef FPC}array [0..15] of byte{$else}TArray0To15OfUInt8{$endif};
    b: {$ifdef FPC}array [0..15] of byte{$else}TArray0To15OfUInt8{$endif};
    rIndex: integer;
  public
    procedure FrameRenderedCB(sender: TBrickletLEDStrip; const length: word);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }
  NUM_LEDS = 16;

var
  e: TExample;

{ Frame rendered callback, is called when a new frame was rendered }
{ We increase the index of one blue LED with every frame }
procedure TExample.FrameRenderedCB(sender: TBrickletLEDStrip; const length: word);
begin
  b[rIndex] := 0;
  if rIndex = NUM_LEDS-1 then begin
    rIndex := 0
  end
  else begin
    rIndex := rIndex + 1
  end;
  b[rIndex] := 255;

  { Set new data for next render cycle }
  sender.SetRGBValues(0, NUM_LEDS, r, g, b);
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  ledStrip := TBrickletLEDStrip.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Set frame duration to 50ms (20 frames per second) }
  ledStrip.SetFrameDuration(50);

  { Register frame rendered callback to function FrameRenderedCB }
  ledStrip.OnFrameRendered := {$ifdef FPC}@{$endif}FrameRenderedCB;

  { Set initial rgb values to get started }
  ledStrip.SetRGBValues(0, NUM_LEDS, r, g, b);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
