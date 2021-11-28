/*
 * model_5.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "model_5".
 *
 * Model version              : 1.466
 * Simulink Coder version : 9.1 (R2019a) 23-Nov-2018
 * C source code generated on : Fri Nov 19 11:49:08 2021
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "model_5.h"
#include "model_5_private.h"
#include "model_5_dt.h"

/* Block signals (default storage) */
B_model_5_T model_5_B;

/* Continuous states */
X_model_5_T model_5_X;

/* Block states (default storage) */
DW_model_5_T model_5_DW;

/* Real-time model */
RT_MODEL_model_5_T model_5_M_;
RT_MODEL_model_5_T *const model_5_M = &model_5_M_;

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 2;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  model_5_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; ++i) {
    x[i] += h * f0[i];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void model_5_output(void)
{
  if (rtmIsMajorTimeStep(model_5_M)) {
    /* set solver stop time */
    if (!(model_5_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&model_5_M->solverInfo,
                            ((model_5_M->Timing.clockTickH0 + 1) *
        model_5_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&model_5_M->solverInfo,
                            ((model_5_M->Timing.clockTick0 + 1) *
        model_5_M->Timing.stepSize0 + model_5_M->Timing.clockTickH0 *
        model_5_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(model_5_M)) {
    model_5_M->Timing.t[0] = rtsiGetT(&model_5_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(model_5_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

    /* S-Function Block: model_5/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(model_5_DW.HILReadEncoderTimebase_Task, 1,
        &model_5_DW.HILReadEncoderTimebase_Buffer);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
      } else {
        model_5_B.y_m = model_5_DW.HILReadEncoderTimebase_Buffer;
      }
    }
  }

  /* SignalGenerator: '<Root>/Signal Generator' */
  model_5_B.SignalGenerator = sin(model_5_P.SignalGenerator_Frequency *
    model_5_M->Timing.t[0]) * model_5_P.SignalGenerator_Amplitude;

  /* Gain: '<Root>/Gain' */
  model_5_B.u = model_5_P.Gain_Gain * model_5_B.SignalGenerator;
  if (rtmIsMajorTimeStep(model_5_M)) {
    /* S-Function (hil_write_analog_block): '<Root>/HIL Write Analog' */

    /* S-Function Block: model_5/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      result = hil_write_analog(model_5_DW.HILInitialize_Card,
        &model_5_P.HILWriteAnalog_channels, 1, &model_5_B.u);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
      }
    }

    /* Gain: '<Root>/counts to rad' */
    model_5_B.y_mrad = model_5_P.countstorad_Gain * model_5_B.y_m;
  }

  /* StateSpace: '<S1>/Internal' */
  model_5_B.Internal = 0.0;
  model_5_B.Internal += model_5_P.Internal_C * model_5_X.Internal_CSTATE[1];
  if (rtmIsMajorTimeStep(model_5_M)) {
  }
}

/* Model update function */
void model_5_update(void)
{
  if (rtmIsMajorTimeStep(model_5_M)) {
    rt_ertODEUpdateContinuousStates(&model_5_M->solverInfo);
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
  if (!(++model_5_M->Timing.clockTick0)) {
    ++model_5_M->Timing.clockTickH0;
  }

  model_5_M->Timing.t[0] = rtsiGetSolverStopTime(&model_5_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++model_5_M->Timing.clockTick1)) {
      ++model_5_M->Timing.clockTickH1;
    }

    model_5_M->Timing.t[1] = model_5_M->Timing.clockTick1 *
      model_5_M->Timing.stepSize1 + model_5_M->Timing.clockTickH1 *
      model_5_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void model_5_derivatives(void)
{
  XDot_model_5_T *_rtXdot;
  _rtXdot = ((XDot_model_5_T *) model_5_M->derivs);

  /* Derivatives for StateSpace: '<S1>/Internal' */
  _rtXdot->Internal_CSTATE[0] = 0.0;
  _rtXdot->Internal_CSTATE[1] = 0.0;
  _rtXdot->Internal_CSTATE[0] += model_5_P.Internal_A[0] *
    model_5_X.Internal_CSTATE[0];
  _rtXdot->Internal_CSTATE[1] += model_5_P.Internal_A[1] *
    model_5_X.Internal_CSTATE[0];
  _rtXdot->Internal_CSTATE[0] += model_5_P.Internal_B * model_5_B.u;
}

/* Model initialize function */
void model_5_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: model_5/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("qube_servo2_usb", "0", &model_5_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_5_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_watchdog_clear(model_5_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_5_M, _rt_error_message);
      return;
    }

    if ((model_5_P.HILInitialize_AIPStart && !is_switching) ||
        (model_5_P.HILInitialize_AIPEnter && is_switching)) {
      result = hil_set_analog_input_ranges(model_5_DW.HILInitialize_Card,
        &model_5_P.HILInitialize_AIChannels, 1U,
        &model_5_P.HILInitialize_AILow, &model_5_P.HILInitialize_AIHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }

    if ((model_5_P.HILInitialize_AOPStart && !is_switching) ||
        (model_5_P.HILInitialize_AOPEnter && is_switching)) {
      result = hil_set_analog_output_ranges(model_5_DW.HILInitialize_Card,
        &model_5_P.HILInitialize_AOChannels, 1U,
        &model_5_P.HILInitialize_AOLow, &model_5_P.HILInitialize_AOHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }

    if ((model_5_P.HILInitialize_AOStart && !is_switching) ||
        (model_5_P.HILInitialize_AOEnter && is_switching)) {
      result = hil_write_analog(model_5_DW.HILInitialize_Card,
        &model_5_P.HILInitialize_AOChannels, 1U,
        &model_5_P.HILInitialize_AOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }

    if (model_5_P.HILInitialize_AOReset) {
      result = hil_watchdog_set_analog_expiration_state
        (model_5_DW.HILInitialize_Card, &model_5_P.HILInitialize_AOChannels, 1U,
         &model_5_P.HILInitialize_AOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }

    result = hil_set_digital_directions(model_5_DW.HILInitialize_Card, NULL, 0U,
      &model_5_P.HILInitialize_DOChannels, 1U);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_5_M, _rt_error_message);
      return;
    }

    if ((model_5_P.HILInitialize_DOStart && !is_switching) ||
        (model_5_P.HILInitialize_DOEnter && is_switching)) {
      result = hil_write_digital(model_5_DW.HILInitialize_Card,
        &model_5_P.HILInitialize_DOChannels, 1U, (t_boolean *)
        &model_5_P.HILInitialize_DOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }

    if (model_5_P.HILInitialize_DOReset) {
      result = hil_watchdog_set_digital_expiration_state
        (model_5_DW.HILInitialize_Card, &model_5_P.HILInitialize_DOChannels, 1U,
         (const t_digital_state *) &model_5_P.HILInitialize_DOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }

    if ((model_5_P.HILInitialize_EIPStart && !is_switching) ||
        (model_5_P.HILInitialize_EIPEnter && is_switching)) {
      model_5_DW.HILInitialize_QuadratureModes[0] =
        model_5_P.HILInitialize_EIQuadrature;
      model_5_DW.HILInitialize_QuadratureModes[1] =
        model_5_P.HILInitialize_EIQuadrature;
      result = hil_set_encoder_quadrature_mode(model_5_DW.HILInitialize_Card,
        model_5_P.HILInitialize_EIChannels, 2U, (t_encoder_quadrature_mode *)
        &model_5_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }

    if ((model_5_P.HILInitialize_EIStart && !is_switching) ||
        (model_5_P.HILInitialize_EIEnter && is_switching)) {
      model_5_DW.HILInitialize_InitialEICounts[0] =
        model_5_P.HILInitialize_EIInitial;
      model_5_DW.HILInitialize_InitialEICounts[1] =
        model_5_P.HILInitialize_EIInitial;
      result = hil_set_encoder_counts(model_5_DW.HILInitialize_Card,
        model_5_P.HILInitialize_EIChannels, 2U,
        &model_5_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }

    if ((model_5_P.HILInitialize_OOStart && !is_switching) ||
        (model_5_P.HILInitialize_OOEnter && is_switching)) {
      result = hil_write_other(model_5_DW.HILInitialize_Card,
        model_5_P.HILInitialize_OOChannels, 3U,
        model_5_P.HILInitialize_OOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }

    if (model_5_P.HILInitialize_OOReset) {
      result = hil_watchdog_set_other_expiration_state
        (model_5_DW.HILInitialize_Card, model_5_P.HILInitialize_OOChannels, 3U,
         model_5_P.HILInitialize_OOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_5_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

  /* S-Function Block: model_5/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(model_5_DW.HILInitialize_Card,
      model_5_P.HILReadEncoderTimebase_samples_,
      &model_5_P.HILReadEncoderTimebase_channels, 1,
      &model_5_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_5_M, _rt_error_message);
    }
  }

  /* InitializeConditions for StateSpace: '<S1>/Internal' */
  model_5_X.Internal_CSTATE[0] = model_5_P.Internal_InitialCondition;
  model_5_X.Internal_CSTATE[1] = model_5_P.Internal_InitialCondition;
}

/* Model terminate function */
void model_5_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: model_5/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_digital_outputs = 0;
    t_uint32 num_final_other_outputs = 0;
    hil_task_stop_all(model_5_DW.HILInitialize_Card);
    hil_monitor_stop_all(model_5_DW.HILInitialize_Card);
    is_switching = false;
    if ((model_5_P.HILInitialize_AOTerminate && !is_switching) ||
        (model_5_P.HILInitialize_AOExit && is_switching)) {
      num_final_analog_outputs = 1U;
    }

    if ((model_5_P.HILInitialize_DOTerminate && !is_switching) ||
        (model_5_P.HILInitialize_DOExit && is_switching)) {
      num_final_digital_outputs = 1U;
    }

    if ((model_5_P.HILInitialize_OOTerminate && !is_switching) ||
        (model_5_P.HILInitialize_OOExit && is_switching)) {
      num_final_other_outputs = 3U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_digital_outputs > 0
        || num_final_other_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(model_5_DW.HILInitialize_Card
                         , &model_5_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , NULL, 0
                         , &model_5_P.HILInitialize_DOChannels,
                         num_final_digital_outputs
                         , model_5_P.HILInitialize_OOChannels,
                         num_final_other_outputs
                         , &model_5_P.HILInitialize_AOFinal
                         , NULL
                         , (t_boolean *) &model_5_P.HILInitialize_DOFinal
                         , model_5_P.HILInitialize_OOFinal
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(model_5_DW.HILInitialize_Card,
            &model_5_P.HILInitialize_AOChannels, num_final_analog_outputs,
            &model_5_P.HILInitialize_AOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_digital_outputs > 0) {
          local_result = hil_write_digital(model_5_DW.HILInitialize_Card,
            &model_5_P.HILInitialize_DOChannels, num_final_digital_outputs,
            (t_boolean *) &model_5_P.HILInitialize_DOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_other_outputs > 0) {
          local_result = hil_write_other(model_5_DW.HILInitialize_Card,
            model_5_P.HILInitialize_OOChannels, num_final_other_outputs,
            model_5_P.HILInitialize_OOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(model_5_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(model_5_DW.HILInitialize_Card);
    hil_monitor_delete_all(model_5_DW.HILInitialize_Card);
    hil_close(model_5_DW.HILInitialize_Card);
    model_5_DW.HILInitialize_Card = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  model_5_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  model_5_update();
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
  model_5_initialize();
}

void MdlTerminate(void)
{
  model_5_terminate();
}

/* Registration function */
RT_MODEL_model_5_T *model_5(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)model_5_M, 0,
                sizeof(RT_MODEL_model_5_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&model_5_M->solverInfo, &model_5_M->Timing.simTimeStep);
    rtsiSetTPtr(&model_5_M->solverInfo, &rtmGetTPtr(model_5_M));
    rtsiSetStepSizePtr(&model_5_M->solverInfo, &model_5_M->Timing.stepSize0);
    rtsiSetdXPtr(&model_5_M->solverInfo, &model_5_M->derivs);
    rtsiSetContStatesPtr(&model_5_M->solverInfo, (real_T **)
                         &model_5_M->contStates);
    rtsiSetNumContStatesPtr(&model_5_M->solverInfo,
      &model_5_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&model_5_M->solverInfo,
      &model_5_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&model_5_M->solverInfo,
      &model_5_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&model_5_M->solverInfo,
      &model_5_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&model_5_M->solverInfo, (&rtmGetErrorStatus(model_5_M)));
    rtsiSetRTModelPtr(&model_5_M->solverInfo, model_5_M);
  }

  rtsiSetSimTimeStep(&model_5_M->solverInfo, MAJOR_TIME_STEP);
  model_5_M->intgData.f[0] = model_5_M->odeF[0];
  model_5_M->contStates = ((real_T *) &model_5_X);
  rtsiSetSolverData(&model_5_M->solverInfo, (void *)&model_5_M->intgData);
  rtsiSetSolverName(&model_5_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = model_5_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    model_5_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    model_5_M->Timing.sampleTimes = (&model_5_M->Timing.sampleTimesArray[0]);
    model_5_M->Timing.offsetTimes = (&model_5_M->Timing.offsetTimesArray[0]);

    /* task periods */
    model_5_M->Timing.sampleTimes[0] = (0.0);
    model_5_M->Timing.sampleTimes[1] = (0.001);

    /* task offsets */
    model_5_M->Timing.offsetTimes[0] = (0.0);
    model_5_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(model_5_M, &model_5_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = model_5_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    model_5_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(model_5_M, 60.0);
  model_5_M->Timing.stepSize0 = 0.001;
  model_5_M->Timing.stepSize1 = 0.001;

  /* External mode info */
  model_5_M->Sizes.checksums[0] = (1589579483U);
  model_5_M->Sizes.checksums[1] = (1274394869U);
  model_5_M->Sizes.checksums[2] = (3055018353U);
  model_5_M->Sizes.checksums[3] = (3842755132U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    model_5_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(model_5_M->extModeInfo,
      &model_5_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(model_5_M->extModeInfo, model_5_M->Sizes.checksums);
    rteiSetTPtr(model_5_M->extModeInfo, rtmGetTPtr(model_5_M));
  }

  model_5_M->solverInfoPtr = (&model_5_M->solverInfo);
  model_5_M->Timing.stepSize = (0.001);
  rtsiSetFixedStepSize(&model_5_M->solverInfo, 0.001);
  rtsiSetSolverMode(&model_5_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  model_5_M->blockIO = ((void *) &model_5_B);

  {
    model_5_B.y_m = 0.0;
    model_5_B.SignalGenerator = 0.0;
    model_5_B.u = 0.0;
    model_5_B.y_mrad = 0.0;
    model_5_B.Internal = 0.0;
  }

  /* parameters */
  model_5_M->defaultParam = ((real_T *)&model_5_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &model_5_X;
    model_5_M->contStates = (x);
    (void) memset((void *)&model_5_X, 0,
                  sizeof(X_model_5_T));
  }

  /* states (dwork) */
  model_5_M->dwork = ((void *) &model_5_DW);
  (void) memset((void *)&model_5_DW, 0,
                sizeof(DW_model_5_T));
  model_5_DW.HILInitialize_FilterFrequency[0] = 0.0;
  model_5_DW.HILInitialize_FilterFrequency[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    model_5_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  model_5_M->Sizes.numContStates = (2);/* Number of continuous states */
  model_5_M->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  model_5_M->Sizes.numY = (0);         /* Number of model outputs */
  model_5_M->Sizes.numU = (0);         /* Number of model inputs */
  model_5_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  model_5_M->Sizes.numSampTimes = (2); /* Number of sample times */
  model_5_M->Sizes.numBlocks = (8);    /* Number of blocks */
  model_5_M->Sizes.numBlockIO = (5);   /* Number of block outputs */
  model_5_M->Sizes.numBlockPrms = (82);/* Sum of parameter "widths" */
  return model_5_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
