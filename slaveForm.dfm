object formSlave: TformSlave
  Left = 517
  Top = 249
  Width = 750
  Height = 530
  HorzScrollBar.Tracking = True
  VertScrollBar.Tracking = True
  BorderIcons = [biSystemMenu, biMinimize, biMaximize, biHelp]
  Caption = 'Slave Form'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 8
    Top = 8
    Width = 473
    Height = 449
    ActivePage = tbshtBp
    TabIndex = 0
    TabOrder = 0
    object tbshtBp: TTabSheet
      Caption = 'BP'
      object imgBp: TImage
        Left = 8
        Top = 4
        Width = 449
        Height = 405
        AutoSize = True
      end
      object imgBpSlot0: TImage
        Left = 108
        Top = 4
        Width = 82
        Height = 400
        AutoSize = True
        OnClick = imgBpSlot0Click
      end
      object imgBpSlot1: TImage
        Left = 193
        Top = 4
        Width = 82
        Height = 400
        AutoSize = True
        OnClick = imgBpSlot1Click
      end
      object imgBpSlot2: TImage
        Left = 278
        Top = 4
        Width = 81
        Height = 400
        AutoSize = True
        OnClick = imgBpSlot2Click
      end
      object imgBpSlot3: TImage
        Left = 363
        Top = 4
        Width = 81
        Height = 400
        AutoSize = True
        OnClick = imgBpSlot3Click
      end
    end
    object tbshtBc: TTabSheet
      Caption = 'BC'
      ImageIndex = 1
      object imgBc: TImage
        Left = 16
        Top = 4
        Width = 89
        Height = 400
        AutoSize = True
      end
      object Label1: TLabel
        Left = 120
        Top = 12
        Width = 48
        Height = 13
        Caption = 'Vendor ID'
      end
      object Label2: TLabel
        Left = 120
        Top = 36
        Width = 65
        Height = 13
        Caption = 'Product Code'
      end
      object Label3: TLabel
        Left = 120
        Top = 60
        Width = 81
        Height = 13
        Caption = 'Revision Number'
      end
      object Label4: TLabel
        Left = 120
        Top = 84
        Width = 66
        Height = 13
        Caption = 'Serial Number'
      end
      object Label5: TLabel
        Left = 120
        Top = 108
        Width = 57
        Height = 13
        Caption = 'HW Version'
      end
      object Label6: TLabel
        Left = 120
        Top = 132
        Width = 55
        Height = 13
        Caption = 'FW Version'
      end
      object Label7: TLabel
        Left = 120
        Top = 156
        Width = 56
        Height = 13
        Caption = 'SW Version'
      end
      object Label8: TLabel
        Left = 120
        Top = 180
        Width = 108
        Height = 13
        Caption = 'ADLINK Serial Number'
      end
      object Label9: TLabel
        Left = 120
        Top = 204
        Width = 50
        Height = 13
        Caption = 'Error Code'
      end
      object edBcInfo0: TEdit
        Left = 232
        Top = 8
        Width = 120
        Height = 21
        TabOrder = 0
      end
      object edBcInfo1: TEdit
        Left = 232
        Top = 32
        Width = 120
        Height = 21
        TabOrder = 1
      end
      object edBcInfo2: TEdit
        Left = 232
        Top = 56
        Width = 120
        Height = 21
        TabOrder = 2
      end
      object edBcInfo3: TEdit
        Left = 232
        Top = 80
        Width = 120
        Height = 21
        TabOrder = 3
      end
      object edBcInfo4: TEdit
        Left = 232
        Top = 104
        Width = 120
        Height = 21
        TabOrder = 4
      end
      object edBcInfo5: TEdit
        Left = 232
        Top = 128
        Width = 120
        Height = 21
        TabOrder = 5
      end
      object edBcInfo6: TEdit
        Left = 232
        Top = 152
        Width = 120
        Height = 21
        TabOrder = 6
      end
      object edBcInfo7: TEdit
        Left = 232
        Top = 176
        Width = 120
        Height = 21
        TabOrder = 7
      end
      object edBcInfo8: TEdit
        Left = 232
        Top = 200
        Width = 120
        Height = 21
        TabOrder = 8
      end
    end
    object tbshtSlot0: TTabSheet
      Caption = 'Slot 0'
      ImageIndex = 2
      object imgSlot0: TImage
        Left = 16
        Top = 4
        Width = 89
        Height = 400
        AutoSize = True
      end
      object Label10: TLabel
        Left = 120
        Top = 8
        Width = 62
        Height = 13
        Caption = 'Module Type'
      end
      object Label11: TLabel
        Left = 120
        Top = 32
        Width = 62
        Height = 13
        Caption = 'Module Ident'
      end
      object Label12: TLabel
        Left = 120
        Top = 56
        Width = 57
        Height = 13
        Caption = 'HW Version'
      end
      object Label13: TLabel
        Left = 120
        Top = 80
        Width = 55
        Height = 13
        Caption = 'FW Version'
      end
      object Label14: TLabel
        Left = 120
        Top = 104
        Width = 108
        Height = 13
        Caption = 'ADLINK Serial Number'
      end
      object Label30: TLabel
        Left = 120
        Top = 128
        Width = 50
        Height = 13
        Caption = 'Error Code'
      end
      object edMod0: TEdit
        Left = 232
        Top = 32
        Width = 120
        Height = 21
        TabOrder = 0
      end
      object edHwVer0: TEdit
        Left = 232
        Top = 56
        Width = 120
        Height = 21
        TabOrder = 1
      end
      object edSwVer0: TEdit
        Left = 232
        Top = 80
        Width = 120
        Height = 21
        TabOrder = 2
      end
      object edAdlinkSerial0: TEdit
        Left = 232
        Top = 104
        Width = 120
        Height = 21
        TabOrder = 3
      end
      object edErr0: TEdit
        Left = 232
        Top = 128
        Width = 120
        Height = 21
        TabOrder = 4
      end
      object edModType0: TEdit
        Left = 232
        Top = 8
        Width = 120
        Height = 21
        TabOrder = 5
      end
    end
    object tbshtSlot1: TTabSheet
      Caption = 'Slot 1'
      ImageIndex = 3
      object imgSlot1: TImage
        Left = 16
        Top = 4
        Width = 89
        Height = 400
        AutoSize = True
      end
      object Label15: TLabel
        Left = 120
        Top = 8
        Width = 62
        Height = 13
        Caption = 'Module Type'
      end
      object Label16: TLabel
        Left = 120
        Top = 32
        Width = 62
        Height = 13
        Caption = 'Module Ident'
      end
      object Label17: TLabel
        Left = 120
        Top = 56
        Width = 57
        Height = 13
        Caption = 'HW Version'
      end
      object Label18: TLabel
        Left = 120
        Top = 80
        Width = 55
        Height = 13
        Caption = 'FW Version'
      end
      object Label19: TLabel
        Left = 120
        Top = 104
        Width = 108
        Height = 13
        Caption = 'ADLINK Serial Number'
      end
      object Label31: TLabel
        Left = 120
        Top = 128
        Width = 50
        Height = 13
        Caption = 'Error Code'
      end
      object edMod1: TEdit
        Left = 232
        Top = 32
        Width = 120
        Height = 21
        TabOrder = 0
      end
      object edHwVer1: TEdit
        Left = 232
        Top = 56
        Width = 120
        Height = 21
        TabOrder = 1
      end
      object edSwVer1: TEdit
        Left = 232
        Top = 80
        Width = 120
        Height = 21
        TabOrder = 2
      end
      object edAdlinkSerial1: TEdit
        Left = 232
        Top = 104
        Width = 120
        Height = 21
        TabOrder = 3
      end
      object edErr1: TEdit
        Left = 232
        Top = 128
        Width = 120
        Height = 21
        TabOrder = 4
      end
      object edModType1: TEdit
        Left = 232
        Top = 8
        Width = 120
        Height = 21
        TabOrder = 5
      end
    end
    object tbshtSlot2: TTabSheet
      Caption = 'Slot 2'
      ImageIndex = 4
      object imgSlot2: TImage
        Left = 16
        Top = 4
        Width = 89
        Height = 400
        AutoSize = True
      end
      object Label20: TLabel
        Left = 120
        Top = 8
        Width = 62
        Height = 13
        Caption = 'Module Type'
      end
      object Label21: TLabel
        Left = 120
        Top = 32
        Width = 62
        Height = 13
        Caption = 'Module Ident'
      end
      object Label22: TLabel
        Left = 120
        Top = 56
        Width = 57
        Height = 13
        Caption = 'HW Version'
      end
      object Label23: TLabel
        Left = 120
        Top = 80
        Width = 55
        Height = 13
        Caption = 'FW Version'
      end
      object Label24: TLabel
        Left = 120
        Top = 104
        Width = 108
        Height = 13
        Caption = 'ADLINK Serial Number'
      end
      object Label32: TLabel
        Left = 120
        Top = 128
        Width = 50
        Height = 13
        Caption = 'Error Code'
      end
      object edMod2: TEdit
        Left = 232
        Top = 32
        Width = 120
        Height = 21
        TabOrder = 0
      end
      object edHwVer2: TEdit
        Left = 232
        Top = 56
        Width = 120
        Height = 21
        TabOrder = 1
      end
      object edSwVer2: TEdit
        Left = 232
        Top = 80
        Width = 120
        Height = 21
        TabOrder = 2
      end
      object edAdlinkSerial2: TEdit
        Left = 232
        Top = 104
        Width = 120
        Height = 21
        TabOrder = 3
      end
      object edErr2: TEdit
        Left = 232
        Top = 128
        Width = 120
        Height = 21
        TabOrder = 4
      end
      object edModType2: TEdit
        Left = 232
        Top = 8
        Width = 120
        Height = 21
        TabOrder = 5
      end
    end
    object tbshtSlot3: TTabSheet
      Caption = 'Slot 3'
      ImageIndex = 5
      object imgSlot3: TImage
        Left = 16
        Top = 4
        Width = 89
        Height = 400
        AutoSize = True
      end
      object Label25: TLabel
        Left = 120
        Top = 8
        Width = 62
        Height = 13
        Caption = 'Module Type'
      end
      object Label26: TLabel
        Left = 120
        Top = 32
        Width = 62
        Height = 13
        Caption = 'Module Ident'
      end
      object Label27: TLabel
        Left = 120
        Top = 56
        Width = 57
        Height = 13
        Caption = 'HW Version'
      end
      object Label28: TLabel
        Left = 120
        Top = 80
        Width = 55
        Height = 13
        Caption = 'FW Version'
      end
      object Label29: TLabel
        Left = 120
        Top = 104
        Width = 108
        Height = 13
        Caption = 'ADLINK Serial Number'
      end
      object Label33: TLabel
        Left = 120
        Top = 128
        Width = 50
        Height = 13
        Caption = 'Error Code'
      end
      object edMod3: TEdit
        Left = 232
        Top = 32
        Width = 120
        Height = 21
        TabOrder = 0
      end
      object edHwVer3: TEdit
        Left = 232
        Top = 56
        Width = 120
        Height = 21
        TabOrder = 1
      end
      object edSwVer3: TEdit
        Left = 232
        Top = 80
        Width = 120
        Height = 21
        TabOrder = 2
      end
      object edAdlinkSerial3: TEdit
        Left = 232
        Top = 104
        Width = 120
        Height = 21
        TabOrder = 3
      end
      object edErr3: TEdit
        Left = 232
        Top = 128
        Width = 120
        Height = 21
        TabOrder = 4
      end
      object edModType3: TEdit
        Left = 232
        Top = 8
        Width = 120
        Height = 21
        TabOrder = 5
      end
    end
    object tbshtDcDiag: TTabSheet
      Caption = 'DC Diag'
      ImageIndex = 6
      object imgDcPara: TImage
        Left = 8
        Top = 4
        Width = 449
        Height = 405
        AutoSize = True
      end
    end
    object tbshtDcPara: TTabSheet
      Caption = 'DC Para.'
      ImageIndex = 7
      object gbPdo: TGroupBox
        Left = 8
        Top = 0
        Width = 441
        Height = 297
        TabOrder = 0
        object Label34: TLabel
          Left = 8
          Top = 32
          Width = 102
          Height = 13
          Caption = 'Synchronization Type'
        end
        object Label35: TLabel
          Left = 8
          Top = 56
          Width = 52
          Height = 13
          Caption = 'Cycle Time'
        end
        object Label36: TLabel
          Left = 8
          Top = 80
          Width = 159
          Height = 13
          Caption = 'Synchronization Types Supported'
        end
        object Label37: TLabel
          Left = 8
          Top = 104
          Width = 96
          Height = 13
          Caption = 'Minimum Cycle Time'
        end
        object Label45: TLabel
          Left = 184
          Top = 8
          Width = 39
          Height = 13
          Caption = 'Rx PDO'
        end
        object Label46: TLabel
          Left = 315
          Top = 8
          Width = 38
          Height = 13
          Caption = 'Tx PDO'
        end
        object Label38: TLabel
          Left = 8
          Top = 128
          Width = 98
          Height = 13
          Caption = 'Calc. and Copy Time'
        end
        object Label39: TLabel
          Left = 8
          Top = 152
          Width = 72
          Height = 13
          Caption = 'Get Cycle Time'
        end
        object Label40: TLabel
          Left = 8
          Top = 176
          Width = 53
          Height = 13
          Caption = 'Delay Time'
        end
        object Label41: TLabel
          Left = 8
          Top = 200
          Width = 85
          Height = 13
          Caption = 'Sync0 Cycle Time'
        end
        object Label42: TLabel
          Left = 8
          Top = 224
          Width = 83
          Height = 13
          Caption = 'SM-Event Missed'
        end
        object Label43: TLabel
          Left = 8
          Top = 248
          Width = 102
          Height = 13
          Caption = 'Cycle Time Too Small'
        end
        object Label44: TLabel
          Left = 8
          Top = 272
          Width = 49
          Height = 13
          Caption = 'Sync Error'
        end
        object edPdoData0: TEdit
          Left = 184
          Top = 32
          Width = 120
          Height = 21
          TabOrder = 0
        end
        object edPdoData1: TEdit
          Left = 184
          Top = 56
          Width = 120
          Height = 21
          TabOrder = 1
        end
        object edPdoData2: TEdit
          Left = 184
          Top = 80
          Width = 120
          Height = 21
          TabOrder = 2
        end
        object edPdoData3: TEdit
          Left = 184
          Top = 104
          Width = 120
          Height = 21
          TabOrder = 3
        end
        object edPdoData4: TEdit
          Left = 184
          Top = 128
          Width = 120
          Height = 21
          TabOrder = 4
        end
        object edPdoData5: TEdit
          Left = 184
          Top = 152
          Width = 120
          Height = 21
          TabOrder = 5
        end
        object edPdoData6: TEdit
          Left = 184
          Top = 176
          Width = 120
          Height = 21
          TabOrder = 6
        end
        object edPdoData7: TEdit
          Left = 184
          Top = 200
          Width = 120
          Height = 21
          TabOrder = 7
        end
        object edPdoData8: TEdit
          Left = 184
          Top = 224
          Width = 120
          Height = 21
          TabOrder = 8
        end
        object edPdoData9: TEdit
          Left = 184
          Top = 248
          Width = 120
          Height = 21
          TabOrder = 9
        end
        object edPdoData10: TEdit
          Left = 184
          Top = 272
          Width = 120
          Height = 21
          TabOrder = 10
        end
        object edPdoData11: TEdit
          Left = 313
          Top = 32
          Width = 120
          Height = 21
          TabOrder = 11
        end
        object edPdoData12: TEdit
          Left = 312
          Top = 56
          Width = 120
          Height = 21
          TabOrder = 12
        end
        object edPdoData13: TEdit
          Left = 312
          Top = 80
          Width = 120
          Height = 21
          TabOrder = 13
        end
        object edPdoData14: TEdit
          Left = 312
          Top = 104
          Width = 120
          Height = 21
          TabOrder = 14
        end
        object edPdoData15: TEdit
          Left = 312
          Top = 128
          Width = 120
          Height = 21
          TabOrder = 15
        end
        object edPdoData16: TEdit
          Left = 312
          Top = 152
          Width = 120
          Height = 21
          TabOrder = 16
        end
        object edPdoData17: TEdit
          Left = 312
          Top = 176
          Width = 120
          Height = 21
          TabOrder = 17
        end
        object edPdoData18: TEdit
          Left = 312
          Top = 200
          Width = 120
          Height = 21
          TabOrder = 18
        end
        object edPdoData19: TEdit
          Left = 312
          Top = 224
          Width = 120
          Height = 21
          TabOrder = 19
        end
        object edPdoData20: TEdit
          Left = 312
          Top = 248
          Width = 120
          Height = 21
          TabOrder = 20
        end
        object edPdoData21: TEdit
          Left = 312
          Top = 272
          Width = 120
          Height = 21
          TabOrder = 21
        end
      end
      object gbDcPara: TGroupBox
        Left = 8
        Top = 296
        Width = 441
        Height = 113
        TabOrder = 1
        object Label47: TLabel
          Left = 8
          Top = 12
          Width = 102
          Height = 13
          Caption = 'DC Parameter Setting'
        end
        object Label48: TLabel
          Left = 8
          Top = 36
          Width = 58
          Height = 13
          Caption = 'Cyclic Mode'
        end
        object Label49: TLabel
          Left = 8
          Top = 60
          Width = 62
          Height = 13
          Caption = 'Output Delay'
        end
        object Label50: TLabel
          Left = 8
          Top = 84
          Width = 54
          Height = 13
          Caption = 'Input Delay'
        end
        object edPdoData22: TEdit
          Left = 185
          Top = 12
          Width = 120
          Height = 21
          TabOrder = 0
        end
        object edPdoData23: TEdit
          Left = 185
          Top = 36
          Width = 120
          Height = 21
          TabOrder = 1
        end
        object edPdoData24: TEdit
          Left = 185
          Top = 60
          Width = 120
          Height = 21
          TabOrder = 2
        end
        object edPdoData25: TEdit
          Left = 184
          Top = 84
          Width = 120
          Height = 21
          TabOrder = 3
        end
      end
    end
    object tbshtErrLog: TTabSheet
      Caption = 'Error Log'
      ImageIndex = 8
      object richEdErr: TRichEdit
        Left = 216
        Top = 16
        Width = 241
        Height = 361
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object vleErrLog: TValueListEditor
        Left = 8
        Top = 16
        Width = 201
        Height = 393
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goAlwaysShowEditor, goThumbTracking]
        ScrollBars = ssVertical
        Strings.Strings = (
          '=')
        TabOrder = 1
        TitleCaptions.Strings = (
          'Index'
          'Value')
        OnSelectCell = vleErrLogSelectCell
        ColWidths = (
          41
          154)
      end
      object btnRefresh: TButton
        Left = 232
        Top = 384
        Width = 100
        Height = 25
        Caption = 'Refresh'
        TabOrder = 2
        OnClick = btnRefreshClick
      end
      object btnClear: TButton
        Left = 344
        Top = 384
        Width = 100
        Height = 25
        Caption = 'Clear'
        TabOrder = 3
        OnClick = btnClearClick
      end
    end
  end
  object btnFoe: TBitBtn
    Left = 380
    Top = 464
    Width = 100
    Height = 25
    Caption = 'FOE'
    ModalResult = 8
    TabOrder = 1
    OnClick = btnFoeClick
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5F5F5F5F5F5F
      5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F3B3B3B2C2C2C5F5F5F5F5F5F5F5F5F5F5F
      5F5F5F5F5F5F5F5F5F5F0000001D1D1D0505050000000505052525255A5A5AB3
      B3B39696966161612727270404040000000101011F1F1F000000000000BFBFBF
      F9F9F9E2E2E2FCFCFCFFFFFFECECECFFFFFFD7D7D7E7E7E7FFFFFFFAFAFADBDB
      DBF4F4F4E7E7E7000000000000BFBFBFAEAEAE7B7B7B7E7E7E989898AAAAAAFF
      FFFFD7D7D7C7C7C79A9A9A8686867474748F8F8FE3E3E3000000000000BFBFBF
      A9A9A97979797E7E7E989898A9A9A9EAEAEAC4C4C4C6C6C69A9A9A8686867474
      748F8F8FE3E3E3000000000000BFBFBFA9A9A97979797E7E7E989898A5A5A5B7
      B7B7979797C4C4C49A9A9A8686867474748F8F8FE3E3E3000000000000BFBFBF
      A9A9A97979797E7E7E989898AAAAAAB7B7B7979797CACACA9C9C9C8686867474
      748F8F8FE3E3E3000000000000BFBFBFAEAEAE7F7F7F818181989898A1A1A1B7
      B7B7979797BFBFBF9A9A9A868686777777949494E3E3E3000000000000BFBFBF
      A9A9A97878787D7D7D999999A8A8A8B7B7B7979797C6C6C6A1A1A18A8A8A7777
      77949494E3E3E3000000000000BFBFBFA9A9A9838383858585999999A6A6A6B7
      B7B7979797C2C2C29C9C9C8787877C7C7C8F8F8FE3E3E3000000959595BFBFBF
      FFFFFFF4F4F4F9F9F9FFFFFFCFCFCF7878787A7A7ABCBCBCFFFFFFFFFFFFF8F8
      F8FFFFFFE7E7E77C7C7CE5E5E58B8B8B9191919090908D8D8D898989A7A7A7FF
      FFFFFFFFFFBBBBBB8686868C8C8C8F8F8F9191918F8F8FD0D0D0FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
  end
  object btnGetOd: TBitBtn
    Left = 620
    Top = 464
    Width = 100
    Height = 25
    Caption = 'Get OD'
    TabOrder = 2
    OnClick = btnGetOdClick
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFDAD9D98180814F4D4E1D1A1C1D1A1C4F4D4E818081DAD9D9FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE918F900D0A0C04010304010304
      01030401030401030401030D0A0C918F90FEFEFEFFFFFFFFFFFFFFFFFFFEFEFE
      4D4B4C0401030401030401030401030401030401030401030401030401030401
      034D4B4CFEFEFEFFFFFFFFFFFF918F90040103040103040103040103464445CC
      CBCCCCCBCC615F60040103040103040103040103918F90FFFFFFDAD9D90D0A0C
      040103040103040103040103B4B3B4FFFFFF6A696A0C090B0401030401030401
      030401030D0A0CDAD9D9818081040103040103040103040103040103949293FF
      FFFF7B7A7B0401030401030401030401030401030401038280814F4D4E040103
      0401030401030401030401034D4B4CFFFFFFC1C0C10401030401030401030401
      030401030401034F4D4E1D1A1C0401030401030401030401030401030F0C0EFD
      FDFDFBFBFB0D0A0C0401030401030401030401030401031D1A1C1D1A1C040103
      0401030401030401032523255C5A5BFDFDFDF7F7F7110E100401030401030401
      030401030401031D1A1C4F4D4E0401030401030401030401030E0B0D5F5D5E8B
      898A4846470401030401030401030401030401030401034F4D4E818081040103
      0401030401030401030401030401030603054442431916180401030401030401
      03040103040103828081DAD9D90D0A0C04010304010304010304010304010363
      6263FFFFFFCAC9CA0401030401030401030401030D0A0CDAD9D9FFFFFF918F90
      040103040103040103040103040103302E2FD2D1D27C7B7C0401030401030401
      03040103918F90FFFFFFFFFFFFFEFEFE4D4B4C04010304010304010304010304
      01030401030401030401030401030401034D4B4CFEFEFEFFFFFFFFFFFFFFFFFF
      FEFEFE918F900D0A0C0401030401030401030401030401030401030D0A0C918F
      90FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAD9D98180814F4D4E1D
      1A1C1D1A1C4F4D4E818081DAD9D9FFFFFFFFFFFFFFFFFFFFFFFF}
  end
  object vleCardInfo: TValueListEditor
    Left = 488
    Top = 8
    Width = 233
    Height = 449
    DropDownRows = 50
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    ParentShowHint = False
    ScrollBars = ssVertical
    ShowHint = False
    TabOrder = 3
    TitleCaptions.Strings = (
      'Index'
      'Value')
    ColWidths = (
      87
      140)
  end
  object OpenDialogFoe: TOpenDialog
    Left = 8
    Top = 464
  end
end
