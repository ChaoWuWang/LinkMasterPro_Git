object formMo7002: TformMo7002
  Left = 928
  Top = 106
  Width = 400
  Height = 762
  Caption = 'EPS-7002'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object gbAxisFirst: TGroupBox
    Left = 8
    Top = 8
    Width = 361
    Height = 89
    Caption = 'Axis 0'
    TabOrder = 0
    object Label0: TLabel
      Left = 8
      Top = 20
      Width = 40
      Height = 13
      Caption = 'Encoder'
    end
    object edEncoder0: TEdit
      Left = 56
      Top = 16
      Width = 113
      Height = 21
      TabOrder = 0
    end
    object dbAxis0: TDBoolean
      Left = 8
      Top = 40
      Width = 40
      Height = 40
      DataBindings = <>
      ParentShowHint = False
      TabOrder = 1
      ControlData = {
        04000200220400002204000007000000034D454CC0C0C000000352E30B918FCE
        119DE300AA004BB85101000000BC02905F010005417269616C0100FF00000000
        0000000000000000000000000000000000000300010000000000000000008080
        80000000000000000000024F6E034F6666000352E30B918FCE119DE300AA004B
        B85101000000BC02C0D4010005417269616C000352E30B918FCE119DE300AA00
        4BB85101000000BC02C0D4010005417269616C00000032000000000100006400
        000001000000000000000000}
    end
  end
  object gbAxisSecond: TGroupBox
    Left = 8
    Top = 104
    Width = 361
    Height = 89
    Caption = 'Axis 1'
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 20
      Width = 40
      Height = 13
      Caption = 'Encoder'
    end
    object edEncoder1: TEdit
      Left = 56
      Top = 16
      Width = 113
      Height = 21
      TabOrder = 0
    end
    object dbAxis1: TDBoolean
      Left = 8
      Top = 40
      Width = 40
      Height = 40
      DataBindings = <>
      TabOrder = 1
      ControlData = {
        04000200220400002204000007000000034D454CC0C0C000000352E30B918FCE
        119DE300AA004BB85101000000BC02905F010005417269616C0100FF00000000
        0000000000000000000000000000000000000300010000000000000000008080
        80000000000000000000024F6E034F6666000352E30B918FCE119DE300AA004B
        B85101000000BC02C0D4010005417269616C000352E30B918FCE119DE300AA00
        4BB85101000000BC02C0D4010005417269616C00000032000000000100006400
        000001000000000000000000}
    end
  end
  object btnSet: TButton
    Left = 152
    Top = 688
    Width = 89
    Height = 25
    Caption = 'Set'
    TabOrder = 2
  end
  object sgAxisConfig: TStringGrid
    Left = 8
    Top = 200
    Width = 361
    Height = 473
    TabOrder = 3
    OnMouseDown = sgAxisConfigMouseDown
  end
end
