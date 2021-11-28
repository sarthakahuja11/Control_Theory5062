/*
 * template_hardware.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "template_hardware".
 *
 * Model version              : 1.464
 * Simulink Coder version : 9.1 (R2019a) 23-Nov-2018
 * C source code generated on : Fri Nov 26 11:19:17 2021
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "template_hardware.h"
#include "template_hardware_private.h"
#include "template_hardware_dt.h"

/* Block signals (default storage) */
B_template_hardware_T template_hardware_B;

/* Block states (default storage) */
DW_template_hardware_T template_hardware_DW;

/* Real-time model */
RT_MODEL_template_hardware_T template_hardware_M_;
RT_MODEL_template_hardware_T *const template_hardware_M = &template_hardware_M_;

/* Model output function */
void template_hardware_output(void)
{
  /* S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

  /* S-Function Block: template_hardware/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_read_encoder
      (template_hardware_DW.HILReadEncoderTimebase_Task, 1,
       &template_hardware_DW.HILReadEncoderTimebase_Buffer);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(template_hardware_M, _rt_error_message);
    } else {
      template_hardware_B.y_m =
        template_hardware_DW.HILReadEncoderTimebase_Buffer;
    }
  }

  /* S-Function (hil_write_analog_block): '<Root>/HIL Write Analog' incorporates:
   *  Constant: '<Root>/Constant'
   */

  /* S-Function Block: template_hardware/HIL Write Analog (hil_write_analog_block) */
  {
    t_error result;
    result = hil_write_analog(template_hardware_DW.HILInitialize_Card,
      &template_hardware_P.HILWriteAnalog_channels, 1,
      &template_hardware_P.Constant_Value);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(template_hardware_M, _rt_error_message);
    }
  }

  /* Gain: '<Root>/counts to rad' */
  template_hardware_B.y_mrad = template_hardware_P.countstorad_Gain *
    template_hardware_B.y_m;
}

/* Model update function */
void template_hardware_update(void)
{
  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++template_hardware_M->Timing.clockTick0)) {
    ++template_hardware_M->Timing.clockTickH0;
  }

  template_hardware_M->Timing.t[0] = template_hardware_M->Timing.clockTick0 *
    template_hardware_M->Timing.stepSize0 +
    template_hardware_M->Timing.clockTickH0 *
    template_hardware_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void template_hardware_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: template_hardware/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("qube_servo2_usb", "0",
                      &template_hardware_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(template_hardware_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_watchdog_clear(template_hardware_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(template_hardware_M, _rt_error_message);
      return;
    }

    if ((template_hardware_P.HILInitialize_AIPStart && !is_switching) ||
        (template_hardware_P.HILInitialize_AIPEnter && is_switching)) {
      result = hil_set_analog_input_ranges
        (template_hardware_DW.HILInitialize_Card,
         &template_hardware_P.HILInitialize_AIChannels, 1U,
         &template_hardware_P.HILInitialize_AILow,
         &template_hardware_P.HILInitialize_AIHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }

    if ((template_hardware_P.HILInitialize_AOPStart && !is_switching) ||
        (template_hardware_P.HILInitialize_AOPEnter && is_switching)) {
      result = hil_set_analog_output_ranges
        (template_hardware_DW.HILInitialize_Card,
         &template_hardware_P.HILInitialize_AOChannels, 1U,
         &template_hardware_P.HILInitialize_AOLow,
         &template_hardware_P.HILInitialize_AOHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }

    if ((template_hardware_P.HILInitialize_AOStart && !is_switching) ||
        (template_hardware_P.HILInitialize_AOEnter && is_switching)) {
      result = hil_write_analog(template_hardware_DW.HILInitialize_Card,
        &template_hardware_P.HILInitialize_AOChannels, 1U,
        &template_hardware_P.HILInitialize_AOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }

    if (template_hardware_P.HILInitialize_AOReset) {
      result = hil_watchdog_set_analog_expiration_state
        (template_hardware_DW.HILInitialize_Card,
         &template_hardware_P.HILInitialize_AOChannels, 1U,
         &template_hardware_P.HILInitialize_AOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }

    result = hil_set_digital_directions(template_hardware_DW.HILInitialize_Card,
      NULL, 0U, &template_hardware_P.HILInitialize_DOChannels, 1U);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(template_hardware_M, _rt_error_message);
      return;
    }

    if ((template_hardware_P.HILInitialize_DOStart && !is_switching) ||
        (template_hardware_P.HILInitialize_DOEnter && is_switching)) {
      result = hil_write_digital(template_hardware_DW.HILInitialize_Card,
        &template_hardware_P.HILInitialize_DOChannels, 1U, (t_boolean *)
        &template_hardware_P.HILInitialize_DOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }

    if (template_hardware_P.HILInitialize_DOReset) {
      result = hil_watchdog_set_digital_expiration_state
        (template_hardware_DW.HILInitialize_Card,
         &template_hardware_P.HILInitialize_DOChannels, 1U, (const
          t_digital_state *) &template_hardware_P.HILInitialize_DOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }

    if ((template_hardware_P.HILInitialize_EIPStart && !is_switching) ||
        (template_hardware_P.HILInitialize_EIPEnter && is_switching)) {
      template_hardware_DW.HILInitialize_QuadratureModes[0] =
        template_hardware_P.HILInitialize_EIQuadrature;
      template_hardware_DW.HILInitialize_QuadratureModes[1] =
        template_hardware_P.HILInitialize_EIQuadrature;
      result = hil_set_encoder_quadrature_mode
        (template_hardware_DW.HILInitialize_Card,
         template_hardware_P.HILInitialize_EIChannels, 2U,
         (t_encoder_quadrature_mode *)
         &template_hardware_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }

    if ((template_hardware_P.HILInitialize_EIStart && !is_switching) ||
        (template_hardware_P.HILInitialize_EIEnter && is_switching)) {
      template_hardware_DW.HILInitialize_InitialEICounts[0] =
        template_hardware_P.HILInitialize_EIInitial;
      template_hardware_DW.HILInitialize_InitialEICounts[1] =
        template_hardware_P.HILInitialize_EIInitial;
      result = hil_set_encoder_counts(template_hardware_DW.HILInitialize_Card,
        template_hardware_P.HILInitialize_EIChannels, 2U,
        &template_hardware_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }

    if ((template_hardware_P.HILInitialize_OOStart && !is_switching) ||
        (template_hardware_P.HILInitialize_OOEnter && is_switching)) {
      result = hil_write_other(template_hardware_DW.HILInitialize_Card,
        template_hardware_P.HILInitialize_OOChannels, 3U,
        template_hardware_P.HILInitialize_OOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }

    if (template_hardware_P.HILInitialize_OOReset) {
      result = hil_watchdog_set_other_expiration_state
        (template_hardware_DW.HILInitialize_Card,
         template_hardware_P.HILInitialize_OOChannels, 3U,
         template_hardware_P.HILInitialize_OOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

  /* S-Function Block: template_hardware/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader
      (template_hardware_DW.HILInitialize_Card,
       template_hardware_P.HILReadEncoderTimebase_samples_,
       &template_hardware_P.HILReadEncoderTimebase_channels, 1,
       &template_hardware_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(template_hardware_M, _rt_error_message);
    }
  }
}

/* Model terminate function */
void template_hardware_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: template_hardware/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_digital_outputs = 0;
    t_uint32 num_final_other_outputs = 0;
    hil_task_stop_all(template_hardware_DW.HILInitialize_Card);
    hil_monitor_stop_all(template_hardware_DW.HILInitialize_Card);
    is_switching = false;
    if ((template_hardware_P.HILInitialize_AOTerminate && !is_switching) ||
        (template_hardware_P.HILInitialize_AOExit && is_switching)) {
      num_final_analog_outputs = 1U;
    }

    if ((template_hardware_P.HILInitialize_DOTerminate && !is_switching) ||
        (template_hardware_P.HILInitialize_DOExit && is_switching)) {
      num_final_digital_outputs = 1U;
    }

    if ((template_hardware_P.HILInitialize_OOTerminate && !is_switching) ||
        (template_hardware_P.HILInitialize_OOExit && is_switching)) {
      num_final_other_outputs = 3U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_digital_outputs > 0
        || num_final_other_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(template_hardware_DW.HILInitialize_Card
                         , &template_hardware_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , NULL, 0
                         , &template_hardware_P.HILInitialize_DOChannels,
                         num_final_digital_outputs
                         , template_hardware_P.HILInitialize_OOChannels,
                         num_final_other_outputs
                         , &template_hardware_P.HILInitialize_AOFinal
                         , NULL
                         , (t_boolean *)
                         &template_hardware_P.HILInitialize_DOFinal
                         , template_hardware_P.HILInitialize_OOFinal
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog
            (template_hardware_DW.HILInitialize_Card,
             &template_hardware_P.HILInitialize_AOChannels,
             num_final_analog_outputs,
             &template_hardware_P.HILInitialize_AOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_digital_outputs > 0) {
          local_result = hil_write_digital
            (template_hardware_DW.HILInitialize_Card,
             &template_hardware_P.HILInitialize_DOChannels,
             num_final_digital_outputs, (t_boolean *)
             &template_hardware_P.HILInitialize_DOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_other_outputs > 0) {
          local_result = hil_write_other(template_hardware_DW.HILInitialize_Card,
            template_hardware_P.HILInitialize_OOChannels,
            num_final_other_outputs, template_hardware_P.HILInitialize_OOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(template_hardware_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(template_hardware_DW.HILInitialize_Card);
    hil_monitor_delete_all(template_hardware_DW.HILInitialize_Card);
    hil_close(template_hardware_DW.HILInitialize_Card);
    template_hardware_DW.HILInitialize_Card = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  template_hardware_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  template_hardware_update();
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
  template_hardware_initialize();
}

void MdlTerminate(void)
{
  template_hardware_terminate();
}

/* Registration function */
RT_MODEL_template_hardware_T *template_hardware(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)template_hardware_M, 0,
                sizeof(RT_MODEL_template_hardware_T));

  /* Initialize timing info */
  {
    int_T *mdlTsMap = template_hardware_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    template_hardware_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    template_hardware_M->Timing.sampleTimes =
      (&template_hardware_M->Timing.sampleTimesArray[0]);
    template_hardware_M->Timing.offsetTimes =
      (&template_hardware_M->Timing.offsetTimesArray[0]);

    /* task periods */
    template_hardware_M->Timing.sampleTimes[0] = (0.001);

    /* task offsets */
    template_hardware_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(template_hardware_M, &template_hardware_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = template_hardware_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    template_hardware_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(template_hardware_M, 60.0);
  template_hardware_M->Timing.stepSize0 = 0.001;

  /* External mode info */
  template_hardware_M->Sizes.checksums[0] = (1118233932U);
  template_hardware_M->Sizes.checksums[1] = (3323706071U);
  template_hardware_M->Sizes.checksums[2] = (1476504044U);
  template_hardware_M->Sizes.checksums[3] = (3364717778U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    template_hardware_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(template_hardware_M->extModeInfo,
      &template_hardware_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(template_hardware_M->extModeInfo,
                        template_hardware_M->Sizes.checksums);
    rteiSetTPtr(template_hardware_M->extModeInfo, rtmGetTPtr(template_hardware_M));
  }

  template_hardware_M->solverInfoPtr = (&template_hardware_M->solverInfo);
  template_hardware_M->Timing.stepSize = (0.001);
  rtsiSetFixedStepSize(&template_hardware_M->solverInfo, 0.001);
  rtsiSetSolverMode(&template_hardware_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  template_hardware_M->blockIO = ((void *) &template_hardware_B);

  {
    template_hardware_B.y_m = 0.0;
    template_hardware_B.y_mrad = 0.0;
  }

  /* parameters */
  template_hardware_M->defaultParam = ((real_T *)&template_hardware_P);

  /* states (dwork) */
  template_hardware_M->dwork = ((void *) &template_hardware_DW);
  (void) memset((void *)&template_hardware_DW, 0,
                sizeof(DW_template_hardware_T));
  template_hardware_DW.HILInitialize_FilterFrequency[0] = 0.0;
  template_hardware_DW.HILInitialize_FilterFrequency[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    template_hardware_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  template_hardware_M->Sizes.numContStates = (0);/* Number of continuous states */
  template_hardware_M->Sizes.numY = (0);/* Number of model outputs */
  template_hardware_M->Sizes.numU = (0);/* Number of model inputs */
  template_hardware_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  template_hardware_M->Sizes.numSampTimes = (1);/* Number of sample times */
  template_hardware_M->Sizes.numBlocks = (6);/* Number of blocks */
  template_hardware_M->Sizes.numBlockIO = (2);/* Number of block outputs */
  template_hardware_M->Sizes.numBlockPrms = (75);/* Sum of parameter "widths" */
  return template_hardware_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
