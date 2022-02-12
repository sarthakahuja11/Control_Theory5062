/*
 * untitled.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "untitled".
 *
 * Model version              : 1.0
 * Simulink Coder version : 9.1 (R2019a) 23-Nov-2018
 * C source code generated on : Fri Nov 26 12:56:30 2021
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "untitled.h"
#include "untitled_private.h"
#include "untitled_dt.h"

/* Block signals (default storage) */
B_untitled_T untitled_B;

/* Block states (default storage) */
DW_untitled_T untitled_DW;

/* Real-time model */
RT_MODEL_untitled_T untitled_M_;
RT_MODEL_untitled_T *const untitled_M = &untitled_M_;

/* Model output function */
void untitled_output(void)
{
  /* local block i/o variables */
  real_T rtb_y_m;

  /* S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

  /* S-Function Block: untitled/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_read_encoder(untitled_DW.HILReadEncoderTimebase_Task, 1,
      &untitled_DW.HILReadEncoderTimebase_Buffer);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(untitled_M, _rt_error_message);
    } else {
      rtb_y_m = untitled_DW.HILReadEncoderTimebase_Buffer;
    }
  }

  /* DiscretePulseGenerator: '<Root>/Pulse Generator' */
  untitled_B.r = (untitled_DW.clockTickCounter < untitled_P.PulseGenerator_Duty)
    && (untitled_DW.clockTickCounter >= 0) ? untitled_P.PulseGenerator_Amp : 0.0;
  if (untitled_DW.clockTickCounter >= untitled_P.PulseGenerator_Period - 1.0) {
    untitled_DW.clockTickCounter = 0;
  } else {
    untitled_DW.clockTickCounter++;
  }

  /* End of DiscretePulseGenerator: '<Root>/Pulse Generator' */

  /* Gain: '<Root>/counts to rad' */
  untitled_B.y_s = untitled_P.countstorad_Gain * rtb_y_m;

  /* Gain: '<Root>/Gain' incorporates:
   *  Sum: '<Root>/Sum'
   */
  untitled_B.Gain = (untitled_B.r - untitled_B.y_s) * untitled_P.kd;

  /* DiscreteStateSpace: '<S1>/Internal' */
  {
    untitled_B.Internal = untitled_P.Internal_D*untitled_B.Gain;
    untitled_B.Internal += untitled_P.Internal_C*untitled_DW.Internal_DSTATE;
  }

  /* S-Function (hil_write_analog_block): '<Root>/HIL Write Analog' */

  /* S-Function Block: untitled/HIL Write Analog (hil_write_analog_block) */
  {
    t_error result;
    result = hil_write_analog(untitled_DW.HILInitialize_Card,
      &untitled_P.HILWriteAnalog_channels, 1, &untitled_B.Internal);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(untitled_M, _rt_error_message);
    }
  }
}

/* Model update function */
void untitled_update(void)
{
  /* Update for DiscreteStateSpace: '<S1>/Internal' */
  {
    untitled_DW.Internal_DSTATE = untitled_B.Gain + untitled_P.Internal_A*
      untitled_DW.Internal_DSTATE;
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++untitled_M->Timing.clockTick0)) {
    ++untitled_M->Timing.clockTickH0;
  }

  untitled_M->Timing.t[0] = untitled_M->Timing.clockTick0 *
    untitled_M->Timing.stepSize0 + untitled_M->Timing.clockTickH0 *
    untitled_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void untitled_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: untitled/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("qube_servo2_usb", "0", &untitled_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(untitled_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(untitled_DW.HILInitialize_Card, " ",
      2);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(untitled_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(untitled_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(untitled_M, _rt_error_message);
      return;
    }

    if ((untitled_P.HILInitialize_AIPStart && !is_switching) ||
        (untitled_P.HILInitialize_AIPEnter && is_switching)) {
      result = hil_set_analog_input_ranges(untitled_DW.HILInitialize_Card,
        &untitled_P.HILInitialize_AIChannels, 1U,
        &untitled_P.HILInitialize_AILow, &untitled_P.HILInitialize_AIHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }

    if ((untitled_P.HILInitialize_AOPStart && !is_switching) ||
        (untitled_P.HILInitialize_AOPEnter && is_switching)) {
      result = hil_set_analog_output_ranges(untitled_DW.HILInitialize_Card,
        &untitled_P.HILInitialize_AOChannels, 1U,
        &untitled_P.HILInitialize_AOLow, &untitled_P.HILInitialize_AOHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }

    if ((untitled_P.HILInitialize_AOStart && !is_switching) ||
        (untitled_P.HILInitialize_AOEnter && is_switching)) {
      result = hil_write_analog(untitled_DW.HILInitialize_Card,
        &untitled_P.HILInitialize_AOChannels, 1U,
        &untitled_P.HILInitialize_AOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }

    if (untitled_P.HILInitialize_AOReset) {
      result = hil_watchdog_set_analog_expiration_state
        (untitled_DW.HILInitialize_Card, &untitled_P.HILInitialize_AOChannels,
         1U, &untitled_P.HILInitialize_AOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }

    result = hil_set_digital_directions(untitled_DW.HILInitialize_Card, NULL, 0U,
      &untitled_P.HILInitialize_DOChannels, 1U);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(untitled_M, _rt_error_message);
      return;
    }

    if ((untitled_P.HILInitialize_DOStart && !is_switching) ||
        (untitled_P.HILInitialize_DOEnter && is_switching)) {
      result = hil_write_digital(untitled_DW.HILInitialize_Card,
        &untitled_P.HILInitialize_DOChannels, 1U, (t_boolean *)
        &untitled_P.HILInitialize_DOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }

    if (untitled_P.HILInitialize_DOReset) {
      result = hil_watchdog_set_digital_expiration_state
        (untitled_DW.HILInitialize_Card, &untitled_P.HILInitialize_DOChannels,
         1U, (const t_digital_state *) &untitled_P.HILInitialize_DOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }

    if ((untitled_P.HILInitialize_EIPStart && !is_switching) ||
        (untitled_P.HILInitialize_EIPEnter && is_switching)) {
      untitled_DW.HILInitialize_QuadratureModes[0] =
        untitled_P.HILInitialize_EIQuadrature;
      untitled_DW.HILInitialize_QuadratureModes[1] =
        untitled_P.HILInitialize_EIQuadrature;
      result = hil_set_encoder_quadrature_mode(untitled_DW.HILInitialize_Card,
        untitled_P.HILInitialize_EIChannels, 2U, (t_encoder_quadrature_mode *)
        &untitled_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }

    if ((untitled_P.HILInitialize_EIStart && !is_switching) ||
        (untitled_P.HILInitialize_EIEnter && is_switching)) {
      untitled_DW.HILInitialize_InitialEICounts[0] =
        untitled_P.HILInitialize_EIInitial;
      untitled_DW.HILInitialize_InitialEICounts[1] =
        untitled_P.HILInitialize_EIInitial;
      result = hil_set_encoder_counts(untitled_DW.HILInitialize_Card,
        untitled_P.HILInitialize_EIChannels, 2U,
        &untitled_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }

    if ((untitled_P.HILInitialize_OOStart && !is_switching) ||
        (untitled_P.HILInitialize_OOEnter && is_switching)) {
      result = hil_write_other(untitled_DW.HILInitialize_Card,
        untitled_P.HILInitialize_OOChannels, 3U,
        untitled_P.HILInitialize_OOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }

    if (untitled_P.HILInitialize_OOReset) {
      result = hil_watchdog_set_other_expiration_state
        (untitled_DW.HILInitialize_Card, untitled_P.HILInitialize_OOChannels, 3U,
         untitled_P.HILInitialize_OOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(untitled_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

  /* S-Function Block: untitled/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(untitled_DW.HILInitialize_Card,
      untitled_P.HILReadEncoderTimebase_samples_,
      &untitled_P.HILReadEncoderTimebase_channels, 1,
      &untitled_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(untitled_M, _rt_error_message);
    }
  }

  /* Start for DiscretePulseGenerator: '<Root>/Pulse Generator' */
  untitled_DW.clockTickCounter = 0;

  /* InitializeConditions for DiscreteStateSpace: '<S1>/Internal' */
  untitled_DW.Internal_DSTATE = untitled_P.Internal_InitialCondition;
}

/* Model terminate function */
void untitled_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: untitled/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_digital_outputs = 0;
    t_uint32 num_final_other_outputs = 0;
    hil_task_stop_all(untitled_DW.HILInitialize_Card);
    hil_monitor_stop_all(untitled_DW.HILInitialize_Card);
    is_switching = false;
    if ((untitled_P.HILInitialize_AOTerminate && !is_switching) ||
        (untitled_P.HILInitialize_AOExit && is_switching)) {
      num_final_analog_outputs = 1U;
    }

    if ((untitled_P.HILInitialize_DOTerminate && !is_switching) ||
        (untitled_P.HILInitialize_DOExit && is_switching)) {
      num_final_digital_outputs = 1U;
    }

    if ((untitled_P.HILInitialize_OOTerminate && !is_switching) ||
        (untitled_P.HILInitialize_OOExit && is_switching)) {
      num_final_other_outputs = 3U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_digital_outputs > 0
        || num_final_other_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(untitled_DW.HILInitialize_Card
                         , &untitled_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , NULL, 0
                         , &untitled_P.HILInitialize_DOChannels,
                         num_final_digital_outputs
                         , untitled_P.HILInitialize_OOChannels,
                         num_final_other_outputs
                         , &untitled_P.HILInitialize_AOFinal
                         , NULL
                         , (t_boolean *) &untitled_P.HILInitialize_DOFinal
                         , untitled_P.HILInitialize_OOFinal
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(untitled_DW.HILInitialize_Card,
            &untitled_P.HILInitialize_AOChannels, num_final_analog_outputs,
            &untitled_P.HILInitialize_AOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_digital_outputs > 0) {
          local_result = hil_write_digital(untitled_DW.HILInitialize_Card,
            &untitled_P.HILInitialize_DOChannels, num_final_digital_outputs,
            (t_boolean *) &untitled_P.HILInitialize_DOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_other_outputs > 0) {
          local_result = hil_write_other(untitled_DW.HILInitialize_Card,
            untitled_P.HILInitialize_OOChannels, num_final_other_outputs,
            untitled_P.HILInitialize_OOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(untitled_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(untitled_DW.HILInitialize_Card);
    hil_monitor_delete_all(untitled_DW.HILInitialize_Card);
    hil_close(untitled_DW.HILInitialize_Card);
    untitled_DW.HILInitialize_Card = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  untitled_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  untitled_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  untitled_initialize();
}

void MdlTerminate(void)
{
  untitled_terminate();
}

/* Registration function */
RT_MODEL_untitled_T *untitled(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)untitled_M, 0,
                sizeof(RT_MODEL_untitled_T));

  /* Initialize timing info */
  {
    int_T *mdlTsMap = untitled_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    untitled_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    untitled_M->Timing.sampleTimes = (&untitled_M->Timing.sampleTimesArray[0]);
    untitled_M->Timing.offsetTimes = (&untitled_M->Timing.offsetTimesArray[0]);

    /* task periods */
    untitled_M->Timing.sampleTimes[0] = (0.04);

    /* task offsets */
    untitled_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(untitled_M, &untitled_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = untitled_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    untitled_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(untitled_M, 10.0);
  untitled_M->Timing.stepSize0 = 0.04;

  /* External mode info */
  untitled_M->Sizes.checksums[0] = (2467316835U);
  untitled_M->Sizes.checksums[1] = (44004309U);
  untitled_M->Sizes.checksums[2] = (446792799U);
  untitled_M->Sizes.checksums[3] = (95557905U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    untitled_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(untitled_M->extModeInfo,
      &untitled_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(untitled_M->extModeInfo, untitled_M->Sizes.checksums);
    rteiSetTPtr(untitled_M->extModeInfo, rtmGetTPtr(untitled_M));
  }

  untitled_M->solverInfoPtr = (&untitled_M->solverInfo);
  untitled_M->Timing.stepSize = (0.04);
  rtsiSetFixedStepSize(&untitled_M->solverInfo, 0.04);
  rtsiSetSolverMode(&untitled_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  untitled_M->blockIO = ((void *) &untitled_B);
  (void) memset(((void *) &untitled_B), 0,
                sizeof(B_untitled_T));

  /* parameters */
  untitled_M->defaultParam = ((real_T *)&untitled_P);

  /* states (dwork) */
  untitled_M->dwork = ((void *) &untitled_DW);
  (void) memset((void *)&untitled_DW, 0,
                sizeof(DW_untitled_T));

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    untitled_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  untitled_M->Sizes.numContStates = (0);/* Number of continuous states */
  untitled_M->Sizes.numY = (0);        /* Number of model outputs */
  untitled_M->Sizes.numU = (0);        /* Number of model inputs */
  untitled_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  untitled_M->Sizes.numSampTimes = (1);/* Number of sample times */
  untitled_M->Sizes.numBlocks = (9);   /* Number of blocks */
  untitled_M->Sizes.numBlockIO = (4);  /* Number of block outputs */
  untitled_M->Sizes.numBlockPrms = (83);/* Sum of parameter "widths" */
  return untitled_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
