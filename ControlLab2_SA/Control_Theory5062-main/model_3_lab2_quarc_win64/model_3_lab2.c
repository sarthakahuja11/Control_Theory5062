/*
 * model_3_lab2.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "model_3_lab2".
 *
 * Model version              : 1.477
 * Simulink Coder version : 9.1 (R2019a) 23-Nov-2018
 * C source code generated on : Fri Nov 19 12:54:23 2021
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "model_3_lab2.h"
#include "model_3_lab2_private.h"
#include "model_3_lab2_dt.h"

/* Block signals (default storage) */
B_model_3_lab2_T model_3_lab2_B;

/* Block states (default storage) */
DW_model_3_lab2_T model_3_lab2_DW;

/* Real-time model */
RT_MODEL_model_3_lab2_T model_3_lab2_M_;
RT_MODEL_model_3_lab2_T *const model_3_lab2_M = &model_3_lab2_M_;

/* Model output function */
void model_3_lab2_output(void)
{
  /* S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

  /* S-Function Block: model_3_lab2/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_read_encoder(model_3_lab2_DW.HILReadEncoderTimebase_Task,
      1, &model_3_lab2_DW.HILReadEncoderTimebase_Buffer);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
    } else {
      model_3_lab2_B.y_m = model_3_lab2_DW.HILReadEncoderTimebase_Buffer;
    }
  }

  /* DiscretePulseGenerator: '<Root>/Pulse Generator' */
  model_3_lab2_B.r = (model_3_lab2_DW.clockTickCounter <
                      model_3_lab2_P.PulseGenerator_Duty) &&
    (model_3_lab2_DW.clockTickCounter >= 0) ? model_3_lab2_P.PulseGenerator_Amp :
    0.0;
  if (model_3_lab2_DW.clockTickCounter >= model_3_lab2_P.PulseGenerator_Period -
      1.0) {
    model_3_lab2_DW.clockTickCounter = 0;
  } else {
    model_3_lab2_DW.clockTickCounter++;
  }

  /* End of DiscretePulseGenerator: '<Root>/Pulse Generator' */

  /* Gain: '<Root>/counts to rad' */
  model_3_lab2_B.y_s = model_3_lab2_P.countstorad_Gain * model_3_lab2_B.y_m;

  /* Sum: '<Root>/Sum' */
  model_3_lab2_B.e = model_3_lab2_B.r - model_3_lab2_B.y_s;

  /* Gain: '<Root>/Gain' */
  model_3_lab2_B.u = model_3_lab2_P.kb * model_3_lab2_B.e;

  /* DiscreteStateSpace: '<S1>/Internal' */
  {
    model_3_lab2_B.Internal = (model_3_lab2_P.Internal_C)*
      model_3_lab2_DW.Internal_DSTATE;
    model_3_lab2_B.Internal += model_3_lab2_P.Internal_D*model_3_lab2_B.u;
  }

  /* S-Function (hil_write_analog_block): '<Root>/HIL Write Analog' */

  /* S-Function Block: model_3_lab2/HIL Write Analog (hil_write_analog_block) */
  {
    t_error result;
    result = hil_write_analog(model_3_lab2_DW.HILInitialize_Card,
      &model_3_lab2_P.HILWriteAnalog_channels, 1, &model_3_lab2_B.Internal);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
    }
  }
}

/* Model update function */
void model_3_lab2_update(void)
{
  /* Update for DiscreteStateSpace: '<S1>/Internal' */
  {
    real_T xnew[1];
    xnew[0] = model_3_lab2_P.Internal_A*model_3_lab2_DW.Internal_DSTATE;
    xnew[0] += model_3_lab2_P.Internal_B*model_3_lab2_B.u;
    (void) memcpy(&model_3_lab2_DW.Internal_DSTATE, xnew,
                  sizeof(real_T)*1);
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
  if (!(++model_3_lab2_M->Timing.clockTick0)) {
    ++model_3_lab2_M->Timing.clockTickH0;
  }

  model_3_lab2_M->Timing.t[0] = model_3_lab2_M->Timing.clockTick0 *
    model_3_lab2_M->Timing.stepSize0 + model_3_lab2_M->Timing.clockTickH0 *
    model_3_lab2_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void model_3_lab2_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: model_3_lab2/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("qube_servo2_usb", "0",
                      &model_3_lab2_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(model_3_lab2_DW.HILInitialize_Card,
      " ", 2);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(model_3_lab2_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
      return;
    }

    if ((model_3_lab2_P.HILInitialize_AIPStart && !is_switching) ||
        (model_3_lab2_P.HILInitialize_AIPEnter && is_switching)) {
      result = hil_set_analog_input_ranges(model_3_lab2_DW.HILInitialize_Card,
        &model_3_lab2_P.HILInitialize_AIChannels, 1U,
        &model_3_lab2_P.HILInitialize_AILow,
        &model_3_lab2_P.HILInitialize_AIHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_3_lab2_P.HILInitialize_AOPStart && !is_switching) ||
        (model_3_lab2_P.HILInitialize_AOPEnter && is_switching)) {
      result = hil_set_analog_output_ranges(model_3_lab2_DW.HILInitialize_Card,
        &model_3_lab2_P.HILInitialize_AOChannels, 1U,
        &model_3_lab2_P.HILInitialize_AOLow,
        &model_3_lab2_P.HILInitialize_AOHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_3_lab2_P.HILInitialize_AOStart && !is_switching) ||
        (model_3_lab2_P.HILInitialize_AOEnter && is_switching)) {
      result = hil_write_analog(model_3_lab2_DW.HILInitialize_Card,
        &model_3_lab2_P.HILInitialize_AOChannels, 1U,
        &model_3_lab2_P.HILInitialize_AOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }

    if (model_3_lab2_P.HILInitialize_AOReset) {
      result = hil_watchdog_set_analog_expiration_state
        (model_3_lab2_DW.HILInitialize_Card,
         &model_3_lab2_P.HILInitialize_AOChannels, 1U,
         &model_3_lab2_P.HILInitialize_AOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }

    result = hil_set_digital_directions(model_3_lab2_DW.HILInitialize_Card, NULL,
      0U, &model_3_lab2_P.HILInitialize_DOChannels, 1U);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
      return;
    }

    if ((model_3_lab2_P.HILInitialize_DOStart && !is_switching) ||
        (model_3_lab2_P.HILInitialize_DOEnter && is_switching)) {
      result = hil_write_digital(model_3_lab2_DW.HILInitialize_Card,
        &model_3_lab2_P.HILInitialize_DOChannels, 1U, (t_boolean *)
        &model_3_lab2_P.HILInitialize_DOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }

    if (model_3_lab2_P.HILInitialize_DOReset) {
      result = hil_watchdog_set_digital_expiration_state
        (model_3_lab2_DW.HILInitialize_Card,
         &model_3_lab2_P.HILInitialize_DOChannels, 1U, (const t_digital_state *)
         &model_3_lab2_P.HILInitialize_DOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_3_lab2_P.HILInitialize_EIPStart && !is_switching) ||
        (model_3_lab2_P.HILInitialize_EIPEnter && is_switching)) {
      model_3_lab2_DW.HILInitialize_QuadratureModes[0] =
        model_3_lab2_P.HILInitialize_EIQuadrature;
      model_3_lab2_DW.HILInitialize_QuadratureModes[1] =
        model_3_lab2_P.HILInitialize_EIQuadrature;
      result = hil_set_encoder_quadrature_mode
        (model_3_lab2_DW.HILInitialize_Card,
         model_3_lab2_P.HILInitialize_EIChannels, 2U, (t_encoder_quadrature_mode
          *) &model_3_lab2_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_3_lab2_P.HILInitialize_EIStart && !is_switching) ||
        (model_3_lab2_P.HILInitialize_EIEnter && is_switching)) {
      model_3_lab2_DW.HILInitialize_InitialEICounts[0] =
        model_3_lab2_P.HILInitialize_EIInitial;
      model_3_lab2_DW.HILInitialize_InitialEICounts[1] =
        model_3_lab2_P.HILInitialize_EIInitial;
      result = hil_set_encoder_counts(model_3_lab2_DW.HILInitialize_Card,
        model_3_lab2_P.HILInitialize_EIChannels, 2U,
        &model_3_lab2_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_3_lab2_P.HILInitialize_OOStart && !is_switching) ||
        (model_3_lab2_P.HILInitialize_OOEnter && is_switching)) {
      result = hil_write_other(model_3_lab2_DW.HILInitialize_Card,
        model_3_lab2_P.HILInitialize_OOChannels, 3U,
        model_3_lab2_P.HILInitialize_OOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }

    if (model_3_lab2_P.HILInitialize_OOReset) {
      result = hil_watchdog_set_other_expiration_state
        (model_3_lab2_DW.HILInitialize_Card,
         model_3_lab2_P.HILInitialize_OOChannels, 3U,
         model_3_lab2_P.HILInitialize_OOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

  /* S-Function Block: model_3_lab2/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(model_3_lab2_DW.HILInitialize_Card,
      model_3_lab2_P.HILReadEncoderTimebase_samples_,
      &model_3_lab2_P.HILReadEncoderTimebase_channels, 1,
      &model_3_lab2_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
    }
  }

  /* Start for DiscretePulseGenerator: '<Root>/Pulse Generator' */
  model_3_lab2_DW.clockTickCounter = 0;

  /* InitializeConditions for DiscreteStateSpace: '<S1>/Internal' */
  model_3_lab2_DW.Internal_DSTATE = model_3_lab2_P.Internal_InitialCondition;
}

/* Model terminate function */
void model_3_lab2_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: model_3_lab2/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_digital_outputs = 0;
    t_uint32 num_final_other_outputs = 0;
    hil_task_stop_all(model_3_lab2_DW.HILInitialize_Card);
    hil_monitor_stop_all(model_3_lab2_DW.HILInitialize_Card);
    is_switching = false;
    if ((model_3_lab2_P.HILInitialize_AOTerminate && !is_switching) ||
        (model_3_lab2_P.HILInitialize_AOExit && is_switching)) {
      num_final_analog_outputs = 1U;
    }

    if ((model_3_lab2_P.HILInitialize_DOTerminate && !is_switching) ||
        (model_3_lab2_P.HILInitialize_DOExit && is_switching)) {
      num_final_digital_outputs = 1U;
    }

    if ((model_3_lab2_P.HILInitialize_OOTerminate && !is_switching) ||
        (model_3_lab2_P.HILInitialize_OOExit && is_switching)) {
      num_final_other_outputs = 3U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_digital_outputs > 0
        || num_final_other_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(model_3_lab2_DW.HILInitialize_Card
                         , &model_3_lab2_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , NULL, 0
                         , &model_3_lab2_P.HILInitialize_DOChannels,
                         num_final_digital_outputs
                         , model_3_lab2_P.HILInitialize_OOChannels,
                         num_final_other_outputs
                         , &model_3_lab2_P.HILInitialize_AOFinal
                         , NULL
                         , (t_boolean *) &model_3_lab2_P.HILInitialize_DOFinal
                         , model_3_lab2_P.HILInitialize_OOFinal
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(model_3_lab2_DW.HILInitialize_Card,
            &model_3_lab2_P.HILInitialize_AOChannels, num_final_analog_outputs,
            &model_3_lab2_P.HILInitialize_AOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_digital_outputs > 0) {
          local_result = hil_write_digital(model_3_lab2_DW.HILInitialize_Card,
            &model_3_lab2_P.HILInitialize_DOChannels, num_final_digital_outputs,
            (t_boolean *) &model_3_lab2_P.HILInitialize_DOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_other_outputs > 0) {
          local_result = hil_write_other(model_3_lab2_DW.HILInitialize_Card,
            model_3_lab2_P.HILInitialize_OOChannels, num_final_other_outputs,
            model_3_lab2_P.HILInitialize_OOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(model_3_lab2_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(model_3_lab2_DW.HILInitialize_Card);
    hil_monitor_delete_all(model_3_lab2_DW.HILInitialize_Card);
    hil_close(model_3_lab2_DW.HILInitialize_Card);
    model_3_lab2_DW.HILInitialize_Card = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  model_3_lab2_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  model_3_lab2_update();
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
  model_3_lab2_initialize();
}

void MdlTerminate(void)
{
  model_3_lab2_terminate();
}

/* Registration function */
RT_MODEL_model_3_lab2_T *model_3_lab2(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)model_3_lab2_M, 0,
                sizeof(RT_MODEL_model_3_lab2_T));

  /* Initialize timing info */
  {
    int_T *mdlTsMap = model_3_lab2_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    model_3_lab2_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    model_3_lab2_M->Timing.sampleTimes =
      (&model_3_lab2_M->Timing.sampleTimesArray[0]);
    model_3_lab2_M->Timing.offsetTimes =
      (&model_3_lab2_M->Timing.offsetTimesArray[0]);

    /* task periods */
    model_3_lab2_M->Timing.sampleTimes[0] = (0.04);

    /* task offsets */
    model_3_lab2_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(model_3_lab2_M, &model_3_lab2_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = model_3_lab2_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    model_3_lab2_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(model_3_lab2_M, 60.0);
  model_3_lab2_M->Timing.stepSize0 = 0.04;

  /* External mode info */
  model_3_lab2_M->Sizes.checksums[0] = (2092353752U);
  model_3_lab2_M->Sizes.checksums[1] = (889879194U);
  model_3_lab2_M->Sizes.checksums[2] = (1749710947U);
  model_3_lab2_M->Sizes.checksums[3] = (3571890671U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    model_3_lab2_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(model_3_lab2_M->extModeInfo,
      &model_3_lab2_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(model_3_lab2_M->extModeInfo,
                        model_3_lab2_M->Sizes.checksums);
    rteiSetTPtr(model_3_lab2_M->extModeInfo, rtmGetTPtr(model_3_lab2_M));
  }

  model_3_lab2_M->solverInfoPtr = (&model_3_lab2_M->solverInfo);
  model_3_lab2_M->Timing.stepSize = (0.04);
  rtsiSetFixedStepSize(&model_3_lab2_M->solverInfo, 0.04);
  rtsiSetSolverMode(&model_3_lab2_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  model_3_lab2_M->blockIO = ((void *) &model_3_lab2_B);

  {
    model_3_lab2_B.y_m = 0.0;
    model_3_lab2_B.r = 0.0;
    model_3_lab2_B.y_s = 0.0;
    model_3_lab2_B.e = 0.0;
    model_3_lab2_B.u = 0.0;
    model_3_lab2_B.Internal = 0.0;
  }

  /* parameters */
  model_3_lab2_M->defaultParam = ((real_T *)&model_3_lab2_P);

  /* states (dwork) */
  model_3_lab2_M->dwork = ((void *) &model_3_lab2_DW);
  (void) memset((void *)&model_3_lab2_DW, 0,
                sizeof(DW_model_3_lab2_T));
  model_3_lab2_DW.Internal_DSTATE = 0.0;
  model_3_lab2_DW.HILInitialize_FilterFrequency[0] = 0.0;
  model_3_lab2_DW.HILInitialize_FilterFrequency[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    model_3_lab2_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  model_3_lab2_M->Sizes.numContStates = (0);/* Number of continuous states */
  model_3_lab2_M->Sizes.numY = (0);    /* Number of model outputs */
  model_3_lab2_M->Sizes.numU = (0);    /* Number of model inputs */
  model_3_lab2_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  model_3_lab2_M->Sizes.numSampTimes = (1);/* Number of sample times */
  model_3_lab2_M->Sizes.numBlocks = (9);/* Number of blocks */
  model_3_lab2_M->Sizes.numBlockIO = (6);/* Number of block outputs */
  model_3_lab2_M->Sizes.numBlockPrms = (84);/* Sum of parameter "widths" */
  return model_3_lab2_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
