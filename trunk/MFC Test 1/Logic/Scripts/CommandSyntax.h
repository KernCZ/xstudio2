#pragma once
#include "../Common.h"
#include "ParameterSyntax.h"

namespace Library
{
   namespace Scripts
   {
      /// <summary>Defines the display group of a script command</summary>
      enum class CommandGroup : UINT;

      enum class CommandType { Standard, Auxiliary, Macro };
      
      /// <summary>Defines the syntax of a script command</summary>
      class CommandSyntax
      {
      public:
         typedef list<ParameterSyntax>  ParameterList;

         /// <summary>Declares command syntax within an external file</summary>
         class Declaration
         {
         public:
            Declaration() : Group(), Versions(0), ID(0) {}

            CommandGroup        Group;
            UINT                Versions, 
                                ID;
            wstring             URL,
                                Syntax;
            list<ParameterType> Params;
         };

         // --------------------- CONSTRUCTION ----------------------

      public:
         CommandSyntax(const Declaration& d);
         virtual ~CommandSyntax();

         // ------------------------ STATIC -------------------------

      private:
         static ParameterList  ParseParams(const wstring& syntax, const list<ParameterType>& params);
         static CommandType    IdentifyType(const UINT id);

         // --------------------- PROPERTIES ------------------------
			
		   // ---------------------- ACCESSORS ------------------------
			
		   // ----------------------- MUTATORS ------------------------

		   // -------------------- REPRESENTATION ---------------------

      public:
         const CommandGroup   Group;
         const CommandType    Type;
         const ParameterList  Parameters;
         const UINT           Versions,
                              ID;
         const wstring        Text,
                              URL;
      };


      
      /// <summary>Defines the display group of a script command</summary>
      enum class CommandGroup : UINT
      {  
         CG_ARRAY             = 0,
         CG_CUSTOM            = 1,
         CG_FLEET             = 2,
         CG_FLOW_CONTROL      = 3,
         CG_GAME_ENGINE       = 4,
         CG_GRAPH             = 5,
         CG_MACRO             = 6,
         CG_MARINE            = 7,
         CG_MATHS             = 8,
         CG_MERCHANT          = 9,
         CG_NON_PLAYER        = 10,
         CG_OBJECT_ACTION     = 11,
         CG_OBJECT_PROPERTY   = 12,
         CG_PASSENGER         = 13,
         CG_PILOT             = 14,
         CG_PLAYER            = 15,
         CG_SCRIPT_PROPERTY   = 16,
         CG_SHIP_ACTION       = 17,
         CG_SHIP_PROPERTY     = 18,
         CG_SHIP_TRADE        = 19,
         CG_SHIP_WING         = 20,
         CG_STATION_PROPERTY  = 21,
         CG_STATION_TRADE     = 22,
         CG_STOCK_EXCHANGE    = 23,
         CG_STRING            = 24,
         CG_SYSTEM_PROPERTY   = 25,
         CG_UNIVERSE_DATA     = 26,
         CG_UNIVERSE_PROPERTY = 27,
         CG_USER_INTERFACE    = 28,
         GC_WAR               = 29,
         CG_WARE_PROPERTY     = 30,
         CG_WEAPON_PROPERTY   = 31,

         CG_HIDDEN            = 32 
      };

      // Number of Command groups
      //
      #define  COMMAND_GROUP_COUNT    33


      
      // Define IDs of some important commands, for clarity
      //
      #define    CMD_NONE                         0
      #define    CMD_COMMENT                      1
      #define    CMD_NOP                          2
      #define    CMD_COMMAND_COMMENT              3
      #define    CMD_END                          4
      #define    CMD_ELSE                         5
      #define    CMD_BREAK                        7
      #define    CMD_CONTINUE                     6
      #define    CMD_GOTO_LABEL                   100
      #define    CMD_DEFINE_LABEL                 101
      #define    CMD_CALL_SCRIPT_VAR_ARGS         102
      #define    CMD_RETURN                       103
      #define    CMD_EXPRESSION                   104
      #define    CMD_RANDOM_VALUE_ZERO            106
      #define    CMD_RANDOM_VALUE                 110
      #define    CMD_INTERRUPT_SCRIPT             111
      #define    CMD_HIDDEN_JUMP                  112
      #define    CMD_CONNECT_SHIP_SCRIPT          113 
      #define    CMD_SET_GLOBAL_SCRIPT_MAP        118
      #define    CMD_IS_DATATYPE                  125
      #define    CMD_READ_TEXT                    126
      #define    CMD_ARRAY_ALLOC                  128
      #define    CMD_ARRAY_ACCESS                 129
      #define    CMD_ARRAY_ASSIGNMENT             130
      #define    CMD_SIZE_OF_ARRAY                131
      #define    CMD_GET_NEWS_ARTICLE_STATE       138
      #define    CMD_SET_NEWS_ARTICLE_STATE       139
      #define    CMD_INCREMENT                    142
      #define    CMD_DECREMENT                    143
      #define    CMD_STRING_PRINTF                150
      #define    CMD_SET_SHIP_UPGRADE_SCRIPT      154
      #define    CMD_SET_LOCAL_VARIABLE           155
      #define    CMD_GET_LOCAL_VARIABLE           156
      #define    CMD_SET_GLOBAL_VARIABLE          157
      #define    CMD_GET_GLOBAL_VARIABLE          158
      #define    CMD_INTERRUPT_SCRIPT_ARGS        159
      #define    CMD_SEND_INCOMING_QUESTION       212
      #define    CMD_SEND_INCOMING_QUESTION_EX    213
      #define    CMD_DEFINE_ARRAY                 250
      #define    CMD_FOR_EACH                     251
      #define    CMD_FOR_EACH_COUNTER             252
      #define    CMD_FOR_LOOP                     253
      #define    CMD_ADD_MENU_ITEM_BYREF          254
      #define    CMD_ADD_MENU_INFO_BYREF          255
      #define    CMD_ADD_MENU_HEADING_BYREF       256
      #define    CMD_WRITE_LOGBOOK                403
      #define    CMD_WRITE_LOG                    406
      #define    CMD_AVERAGE_PRICE_BY_REF         628
      #define    CMD_AVERAGE_PRICE                629
      #define    CMD_REGISTER_AL_SCRIPT           1000
      #define    CMD_UNREGISTER_AL_SCRIPT         1001
      #define    CMD_INTERRUPT_TASK_ARGS          1016
      #define    CMD_BEGIN_TASK_ARGS              1017
      #define    CMD_SPEAK_TEXT                   1042
      #define    CMD_IS_SCRIPT_ON_STACK           1072
      #define    CMD_GET_INDEX_OF_ARRAY           1083
      #define    CMD_REGISTER_QUEST_SCRIPT        1084
      #define    CMD_ADD_EVENT_LISTENER           1088
      #define    CMD_DISPLAY_NEWS_ARTICLE         1090
      #define    CMD_REGISTER_HOTKEY_1            1100
      #define    CMD_REGISTER_GOD_EVENT           1141
      #define    CMD_GOTO_SUB                     1167
      #define    CMD_END_SUB                      1168
      #define    CMD_DELAYED_COMMAND              1185
      #define    CMD_CONNECT_WING_SCRIPT          1186
      #define    CMD_READ_TEXT_ARRAY              1337
      #define    CMD_READ_TEXT_EXISTS             1338
      #define    CMD_ADD_SECONDARY_SIGNAL         1339
      #define    CMD_SET_WING_UPGRADE_SCRIPT      1381
      #define    CMD_SET_GLOBAL_WING_SCRIPT_MAP   1382
      #define    CMD_SET_WING_COMMAND             1384
      #define    CMD_ADD_WING_SCRIPT              1393
      #define    CMD_ADD_WING_SCRIPT_CHECK        1398
      #define    CMD_SET_GLOBAL_SECONDARY_MAP     1407
      #define    CMD_GET_LOCAL_VARIABLE_KEYS      1414
      #define    CMD_CALL_SCRIPT_ARGS             1425
      #define    CMD_LAUNCH_SCRIPT_ARGS           1426
      #define    CMD_GET_GLOBAL_VARIABLE_KEYS     1436
      #define    CMD_ADD_MENU_INFO                1446
      #define    CMD_ADD_MENU_ITEM                1447
      #define    CMD_ADD_MENU_HEADING             1448
      #define    CMD_GET_LOCAL_VARIABLE_REGEX     1450
      #define    CMD_GET_GLOBAL_VARIABLE_REGEX    1451
      #define    CMD_SET_SHIP_PRELOAD_SCRIPT      1466
      #define    CMD_SET_WING_PRELOAD_SCRIPT      1467
      #define    CMD_GET_OBJECT_NAME_ARRAY        1496
      #define    CMD_REGISTER_HOTKEY_2            1511
      #define    CMD_ADD_NEWS_ARTICLE             1531
      #define    CMD_ADD_INFO_ARTICLE             1532
      #define    CMD_ADD_HISTORY_ARTICLE          1533
      #define    CMD_ADD_STORIES_ARTICLE          1534
      #define    CMD_ADD_SCRIPT_OPTIONS           1598
      #define    CMD_REMOVE_SCRIPT_OPTIONS        1599
      #define    CMD_CHECK_SCRIPT_EXISTS_1        1600
      #define    CMD_REGISTER_HOTKEY_1_BYREF      1601
      #define    CMD_GET_HOTKEY_INDEX_1           1602
      #define    CMD_REGISTER_HOTKEY_2_BYREF      1603
      #define    CMD_GET_HOTKEY_INDEX_2           1604
      #define    CMD_CHECK_SCRIPT_EXISTS_2        1605
      #define    CMD_ADD_PLAYER_GRAPH             1608
      #define    CMD_REMOVE_PLAYER_GRAPH          1609
      #define    CMD_ADD_CLASS_GRAPH              1610
      #define    CMD_REMOVE_CLASS_GRAPH           1611
      #define    CMD_ADD_OBJECT_GRAPH             1612
      #define    CMD_REMOVE_OBJECT_GRAPH          1613

   }
}
