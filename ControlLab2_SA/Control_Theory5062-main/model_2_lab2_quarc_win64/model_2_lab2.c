/*
 * model_2_lab2.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "model_2_lab2".
 *
 * Model version              : 1.475
 * Simulink Coder version : 9.1 (R2019a) 23-Nov-2018
 * C source code generated on : Fri Nov 19 12:35:53 2021
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "model_2_lab2.h"
#include "model_2_lab2_private.h"
#include "model_2_lab2_dt.h"

/* Block signals (default storage) */
B_model_2_lab2_T model_2_lab2_B;

/* Continuous states */
X_model_2_lab2_T model_2_lab2_X;

/* Block states (default storage) */
DW_model_2_lab2_T model_2_lab2_DW;

/* Real-time model */
RT_MODEL_model_2_lab2_T model_2_lab2_M_;
RT_MODEL_model_2_lab2_T *const model_2_lab2_M = &model_2_lab2_M_;

/*
 * Time delay interpolation routine
 *
 * The linear interpolation is performed using the formula:
 *
 *          (t2 - tMinusDelay)         (tMinusDelay - t1)
 * u(t)  =  ----------------- * u1  +  ------------------- * u2
 *              (t2 - t1)                  (t2 - t1)
 */
real_T rt_TDelayInterpolate(
  real_T tMinusDelay,                 /* tMinusDelay = currentSimTime - delay */
  real_T tStart,
  real_T *tBuf,
  real_T *uBuf,
  int_T bufSz,
  int_T *lastIdx,
  int_T oldestIdx,
  int_T newIdx,
  real_T initOutput,
  boolean_T discrete,
  boolean_T minorStepAndTAtLastMajorOutput)
{
  int_T i;
  real_T yout, t1, t2, u1, u2;

  /*
   * If there is only one data point in the buffer, this data point must be
   * the t= 0 and tMinusDelay > t0, it ask for something unknown. The best
   * guess if initial output as well
   */
  if ((newIdx == 0) && (oldestIdx ==0 ) && (tMinusDelay > tStart))
    return initOutput;

  /*
   * If tMinusDelay is less than zero, should output initial value
   */
  if (tMinusDelay <= tStart)
    return initOutput;

  /* For fixed buffer extrapolation:
   * if tMinusDelay is small than the time at oldestIdx, if discrete, output
   * tailptr value,  else use tailptr and tailptr+1 value to extrapolate
   * It is also for fixed buffer. Note: The same condition can happen for transport delay block where
   * use tStart and and t[tail] other than using t[tail] and t[tail+1].
   * See below
   */
  if ((tMinusDelay <= tBuf[oldestIdx] ) ) {
    if (discrete) {
      return(uBuf[oldestIdx]);
    } else {
      int_T tempIdx= oldestIdx + 1;
      if (oldestIdx == bufSz-1)
        tempIdx = 0;
      t1= tBuf[oldestIdx];
      t2= tBuf[tempIdx];
      u1= uBuf[oldestIdx];
      u2= uBuf[tempIdx];
      if (t2 == t1) {
        if (tMinusDelay >= t2) {
          yout = u2;
        } else {
          yout = u1;
        }
      } else {
        real_T f1 = (t2-tMinusDelay) / (t2-t1);
        real_T f2 = 1.0 - f1;

        /*
         * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
         */
        yout = f1*u1 + f2*u2;
      }

      return yout;
    }
  }

  /*
   * When block does not have direct feedthrough, we use the table of
   * values to extrapolate off the end of the table for delays that are less
   * than 0 (less then step size).  This is not completely accurate.  The
   * chain of events is as follows for a given time t.  Major output - look
   * in table.  Update - add entry to table.  Now, if we call the output at
   * time t again, there is a new entry in the table. For very small delays,
   * this means that we will have a different answer from the previous call
   * to the output fcn at the same time t.  The following code prevents this
   * from happening.
   */
  if (minorStepAndTAtLastMajorOutput) {
    /* pretend that the new entry has not been added to table */
    if (newIdx != 0) {
      if (*lastIdx == newIdx) {
        (*lastIdx)--;
      }

      newIdx--;
    } else {
      if (*lastIdx == newIdx) {
        *lastIdx = bufSz-1;
      }

      newIdx = bufSz - 1;
    }
  }

  i = *lastIdx;
  if (tBuf[i] < tMinusDelay) {
    /* Look forward starting at last index */
    while (tBuf[i] < tMinusDelay) {
      /* May occur if the delay is less than step-size - extrapolate */
      if (i == newIdx)
        break;
      i = ( i < (bufSz-1) ) ? (i+1) : 0;/* move through buffer */
    }
  } else {
    /*
     * Look backwards starting at last index which can happen when the
     * delay time increases.
     */
    while (tBuf[i] >= tMinusDelay) {
      /*
       * Due to the entry condition at top of function, we
       * should never hit the end.
       */
      i = (i > 0) ? i-1 : (bufSz-1);   /* move through buffer */
    }

    i = ( i < (bufSz-1) ) ? (i+1) : 0;
  }

  *lastIdx = i;
  if (discrete) {
    /*
     * tempEps = 128 * eps;
     * localEps = max(tempEps, tempEps*fabs(tBuf[i]))/2;
     */
    double tempEps = (DBL_EPSILON) * 128.0;
    double localEps = tempEps * fabs(tBuf[i]);
    if (tempEps > localEps) {
      localEps = tempEps;
    }

    localEps = localEps / 2.0;
    if (tMinusDelay >= (tBuf[i] - localEps)) {
      yout = uBuf[i];
    } else {
      if (i == 0) {
        yout = uBuf[bufSz-1];
      } else {
        yout = uBuf[i-1];
      }
    }
  } else {
    if (i == 0) {
      t1 = tBuf[bufSz-1];
      u1 = uBuf[bufSz-1];
    } else {
      t1 = tBuf[i-1];
      u1 = uBuf[i-1];
    }

    t2 = tBuf[i];
    u2 = uBuf[i];
    if (t2 == t1) {
      if (tMinusDelay >= t2) {
        yout = u2;
      } else {
        yout = u1;
      }
    } else {
      real_T f1 = (t2-tMinusDelay) / (t2-t1);
      real_T f2 = 1.0 - f1;

      /*
       * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
       */
      yout = f1*u1 + f2*u2;
    }
  }

  return(yout);
}

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
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  model_2_lab2_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; ++i) {
    x[i] += h * f0[i];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void model_2_lab2_output(void)
{
  if (rtmIsMajorTimeStep(model_2_lab2_M)) {
    /* set solver stop time */
    if (!(model_2_lab2_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&model_2_lab2_M->solverInfo,
                            ((model_2_lab2_M->Timing.clockTickH0 + 1) *
        model_2_lab2_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&model_2_lab2_M->solverInfo,
                            ((model_2_lab2_M->Timing.clockTick0 + 1) *
        model_2_lab2_M->Timing.stepSize0 + model_2_lab2_M->Timing.clockTickH0 *
        model_2_lab2_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(model_2_lab2_M)) {
    model_2_lab2_M->Timing.t[0] = rtsiGetT(&model_2_lab2_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(model_2_lab2_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

    /* S-Function Block: model_2_lab2/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(model_2_lab2_DW.HILReadEncoderTimebase_Task,
        1, &model_2_lab2_DW.HILReadEncoderTimebase_Buffer);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
      } else {
        model_2_lab2_B.y_m = model_2_lab2_DW.HILReadEncoderTimebase_Buffer;
      }
    }

    /* DiscretePulseGenerator: '<Root>/Pulse Generator' */
    model_2_lab2_B.r = (model_2_lab2_DW.clockTickCounter <
                        model_2_lab2_P.PulseGenerator_Duty) &&
      (model_2_lab2_DW.clockTickCounter >= 0) ?
      model_2_lab2_P.PulseGenerator_Amp : 0.0;
    if (model_2_lab2_DW.clockTickCounter >= model_2_lab2_P.PulseGenerator_Period
        - 1.0) {
      model_2_lab2_DW.clockTickCounter = 0;
    } else {
      model_2_lab2_DW.clockTickCounter++;
    }

    /* End of DiscretePulseGenerator: '<Root>/Pulse Generator' */

    /* Gain: '<Root>/counts to rad' */
    model_2_lab2_B.y_s = model_2_lab2_P.countstorad_Gain * model_2_lab2_B.y_m;
  }

  /* TransportDelay: '<Root>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &model_2_lab2_DW.TransportDelay_PWORK.TUbufferPtrs[0];
    real_T **tBuffer = (real_T**)
      &model_2_lab2_DW.TransportDelay_PWORK.TUbufferPtrs[1];
    real_T simTime = model_2_lab2_M->Timing.t[0];
    real_T tMinusDelay = simTime - model_2_lab2_P.TransportDelay_Delay;
    if (model_2_lab2_P.TransportDelay_Delay == 0.0)
      model_2_lab2_B.TransportDelay = model_2_lab2_B.y_s;
    else
      model_2_lab2_B.TransportDelay = rt_TDelayInterpolate(
        tMinusDelay,
        0.0,
        *tBuffer,
        *uBuffer,
        model_2_lab2_DW.TransportDelay_IWORK.CircularBufSize,
        &model_2_lab2_DW.TransportDelay_IWORK.Last,
        model_2_lab2_DW.TransportDelay_IWORK.Tail,
        model_2_lab2_DW.TransportDelay_IWORK.Head,
        model_2_lab2_P.TransportDelay_InitOutput,
        1,
        0);
  }

  /* Sum: '<Root>/Sum' */
  model_2_lab2_B.e = model_2_lab2_B.r - model_2_lab2_B.TransportDelay;

  /* Gain: '<Root>/Gain' */
  model_2_lab2_B.u = model_2_lab2_P.kb * model_2_lab2_B.e;

  /* StateSpace: '<S1>/Internal' */
  model_2_lab2_B.Internal = 0.0;
  model_2_lab2_B.Internal += model_2_lab2_P.Internal_C *
    model_2_lab2_X.Internal_CSTATE;
  model_2_lab2_B.Internal += model_2_lab2_P.Internal_D * model_2_lab2_B.u;
  if (rtmIsMajorTimeStep(model_2_lab2_M)) {
    /* S-Function (hil_write_analog_block): '<Root>/HIL Write Analog' */

    /* S-Function Block: model_2_lab2/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      result = hil_write_analog(model_2_lab2_DW.HILInitialize_Card,
        &model_2_lab2_P.HILWriteAnalog_channels, 1, &model_2_lab2_B.Internal);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void model_2_lab2_update(void)
{
  /* Update for TransportDelay: '<Root>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &model_2_lab2_DW.TransportDelay_PWORK.TUbufferPtrs[0];
    real_T **tBuffer = (real_T**)
      &model_2_lab2_DW.TransportDelay_PWORK.TUbufferPtrs[1];
    real_T simTime = model_2_lab2_M->Timing.t[0];
    model_2_lab2_DW.TransportDelay_IWORK.Head =
      ((model_2_lab2_DW.TransportDelay_IWORK.Head <
        (model_2_lab2_DW.TransportDelay_IWORK.CircularBufSize-1)) ?
       (model_2_lab2_DW.TransportDelay_IWORK.Head+1) : 0);
    if (model_2_lab2_DW.TransportDelay_IWORK.Head ==
        model_2_lab2_DW.TransportDelay_IWORK.Tail) {
      model_2_lab2_DW.TransportDelay_IWORK.Tail =
        ((model_2_lab2_DW.TransportDelay_IWORK.Tail <
          (model_2_lab2_DW.TransportDelay_IWORK.CircularBufSize-1)) ?
         (model_2_lab2_DW.TransportDelay_IWORK.Tail+1) : 0);
    }

    (*tBuffer)[model_2_lab2_DW.TransportDelay_IWORK.Head] = simTime;
    (*uBuffer)[model_2_lab2_DW.TransportDelay_IWORK.Head] = model_2_lab2_B.y_s;
  }

  if (rtmIsMajorTimeStep(model_2_lab2_M)) {
    rt_ertODEUpdateContinuousStates(&model_2_lab2_M->solverInfo);
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
  if (!(++model_2_lab2_M->Timing.clockTick0)) {
    ++model_2_lab2_M->Timing.clockTickH0;
  }

  model_2_lab2_M->Timing.t[0] = rtsiGetSolverStopTime
    (&model_2_lab2_M->solverInfo);

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
    if (!(++model_2_lab2_M->Timing.clockTick1)) {
      ++model_2_lab2_M->Timing.clockTickH1;
    }

    model_2_lab2_M->Timing.t[1] = model_2_lab2_M->Timing.clockTick1 *
      model_2_lab2_M->Timing.stepSize1 + model_2_lab2_M->Timing.clockTickH1 *
      model_2_lab2_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void model_2_lab2_derivatives(void)
{
  XDot_model_2_lab2_T *_rtXdot;
  _rtXdot = ((XDot_model_2_lab2_T *) model_2_lab2_M->derivs);

  /* Derivatives for StateSpace: '<S1>/Internal' */
  _rtXdot->Internal_CSTATE = 0.0;
  _rtXdot->Internal_CSTATE += model_2_lab2_P.Internal_A *
    model_2_lab2_X.Internal_CSTATE;
  _rtXdot->Internal_CSTATE += model_2_lab2_P.Internal_B * model_2_lab2_B.u;
}

/* Model initialize function */
void model_2_lab2_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: model_2_lab2/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("qube_servo2_usb", "0",
                      &model_2_lab2_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(model_2_lab2_DW.HILInitialize_Card,
      " ", 2);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(model_2_lab2_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
      return;
    }

    if ((model_2_lab2_P.HILInitialize_AIPStart && !is_switching) ||
        (model_2_lab2_P.HILInitialize_AIPEnter && is_switching)) {
      result = hil_set_analog_input_ranges(model_2_lab2_DW.HILInitialize_Card,
        &model_2_lab2_P.HILInitialize_AIChannels, 1U,
        &model_2_lab2_P.HILInitialize_AILow,
        &model_2_lab2_P.HILInitialize_AIHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_2_lab2_P.HILInitialize_AOPStart && !is_switching) ||
        (model_2_lab2_P.HILInitialize_AOPEnter && is_switching)) {
      result = hil_set_analog_output_ranges(model_2_lab2_DW.HILInitialize_Card,
        &model_2_lab2_P.HILInitialize_AOChannels, 1U,
        &model_2_lab2_P.HILInitialize_AOLow,
        &model_2_lab2_P.HILInitialize_AOHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_2_lab2_P.HILInitialize_AOStart && !is_switching) ||
        (model_2_lab2_P.HILInitialize_AOEnter && is_switching)) {
      result = hil_write_analog(model_2_lab2_DW.HILInitialize_Card,
        &model_2_lab2_P.HILInitialize_AOChannels, 1U,
        &model_2_lab2_P.HILInitialize_AOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }

    if (model_2_lab2_P.HILInitialize_AOReset) {
      result = hil_watchdog_set_analog_expiration_state
        (model_2_lab2_DW.HILInitialize_Card,
         &model_2_lab2_P.HILInitialize_AOChannels, 1U,
         &model_2_lab2_P.HILInitialize_AOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }

    result = hil_set_digital_directions(model_2_lab2_DW.HILInitialize_Card, NULL,
      0U, &model_2_lab2_P.HILInitialize_DOChannels, 1U);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
      return;
    }

    if ((model_2_lab2_P.HILInitialize_DOStart && !is_switching) ||
        (model_2_lab2_P.HILInitialize_DOEnter && is_switching)) {
      result = hil_write_digital(model_2_lab2_DW.HILInitialize_Card,
        &model_2_lab2_P.HILInitialize_DOChannels, 1U, (t_boolean *)
        &model_2_lab2_P.HILInitialize_DOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }

    if (model_2_lab2_P.HILInitialize_DOReset) {
      result = hil_watchdog_set_digital_expiration_state
        (model_2_lab2_DW.HILInitialize_Card,
         &model_2_lab2_P.HILInitialize_DOChannels, 1U, (const t_digital_state *)
         &model_2_lab2_P.HILInitialize_DOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_2_lab2_P.HILInitialize_EIPStart && !is_switching) ||
        (model_2_lab2_P.HILInitialize_EIPEnter && is_switching)) {
      model_2_lab2_DW.HILInitialize_QuadratureModes[0] =
        model_2_lab2_P.HILInitialize_EIQuadrature;
      model_2_lab2_DW.HILInitialize_QuadratureModes[1] =
        model_2_lab2_P.HILInitialize_EIQuadrature;
      result = hil_set_encoder_quadrature_mode
        (model_2_lab2_DW.HILInitialize_Card,
         model_2_lab2_P.HILInitialize_EIChannels, 2U, (t_encoder_quadrature_mode
          *) &model_2_lab2_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_2_lab2_P.HILInitialize_EIStart && !is_switching) ||
        (model_2_lab2_P.HILInitialize_EIEnter && is_switching)) {
      model_2_lab2_DW.HILInitialize_InitialEICounts[0] =
        model_2_lab2_P.HILInitialize_EIInitial;
      model_2_lab2_DW.HILInitialize_InitialEICounts[1] =
        model_2_lab2_P.HILInitialize_EIInitial;
      result = hil_set_encoder_counts(model_2_lab2_DW.HILInitialize_Card,
        model_2_lab2_P.HILInitialize_EIChannels, 2U,
        &model_2_lab2_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }

    if ((model_2_lab2_P.HILInitialize_OOStart && !is_switching) ||
        (model_2_lab2_P.HILInitialize_OOEnter && is_switching)) {
      result = hil_write_other(model_2_lab2_DW.HILInitialize_Card,
        model_2_lab2_P.HILInitialize_OOChannels, 3U,
        model_2_lab2_P.HILInitialize_OOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }

    if (model_2_lab2_P.HILInitialize_OOReset) {
      result = hil_watchdog_set_other_expiration_state
        (model_2_lab2_DW.HILInitialize_Card,
         model_2_lab2_P.HILInitialize_OOChannels, 3U,
         model_2_lab2_P.HILInitialize_OOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<Root>/HIL Read Encoder Timebase' */

  /* S-Function Block: model_2_lab2/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(model_2_lab2_DW.HILInitialize_Card,
      model_2_lab2_P.HILReadEncoderTimebase_samples_,
      &model_2_lab2_P.HILReadEncoderTimebase_channels, 1,
      &model_2_lab2_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
    }
  }

  /* Start for DiscretePulseGenerator: '<Root>/Pulse Generator' */
  model_2_lab2_DW.clockTickCounter = 0;

  /* Start for TransportDelay: '<Root>/Transport Delay' */
  {
    real_T *pBuffer = &model_2_lab2_DW.TransportDelay_RWORK.TUbufferArea[0];
    model_2_lab2_DW.TransportDelay_IWORK.Tail = 0;
    model_2_lab2_DW.TransportDelay_IWORK.Head = 0;
    model_2_lab2_DW.TransportDelay_IWORK.Last = 0;
    model_2_lab2_DW.TransportDelay_IWORK.CircularBufSize = 1024;
    pBuffer[0] = model_2_lab2_P.TransportDelay_InitOutput;
    pBuffer[1024] = model_2_lab2_M->Timing.t[0];
    model_2_lab2_DW.TransportDelay_PWORK.TUbufferPtrs[0] = (void *) &pBuffer[0];
    model_2_lab2_DW.TransportDelay_PWORK.TUbufferPtrs[1] = (void *) &pBuffer
      [1024];
  }

  /* InitializeConditions for StateSpace: '<S1>/Internal' */
  model_2_lab2_X.Internal_CSTATE = model_2_lab2_P.Internal_InitialCondition;
}

/* Model terminate function */
void model_2_lab2_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: model_2_lab2/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_digital_outputs = 0;
    t_uint32 num_final_other_outputs = 0;
    hil_task_stop_all(model_2_lab2_DW.HILInitialize_Card);
    hil_monitor_stop_all(model_2_lab2_DW.HILInitialize_Card);
    is_switching = false;
    if ((model_2_lab2_P.HILInitialize_AOTerminate && !is_switching) ||
        (model_2_lab2_P.HILInitialize_AOExit && is_switching)) {
      num_final_analog_outputs = 1U;
    }

    if ((model_2_lab2_P.HILInitialize_DOTerminate && !is_switching) ||
        (model_2_lab2_P.HILInitialize_DOExit && is_switching)) {
      num_final_digital_outputs = 1U;
    }

    if ((model_2_lab2_P.HILInitialize_OOTerminate && !is_switching) ||
        (model_2_lab2_P.HILInitialize_OOExit && is_switching)) {
      num_final_other_outputs = 3U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_digital_outputs > 0
        || num_final_other_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(model_2_lab2_DW.HILInitialize_Card
                         , &model_2_lab2_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , NULL, 0
                         , &model_2_lab2_P.HILInitialize_DOChannels,
                         num_final_digital_outputs
                         , model_2_lab2_P.HILInitialize_OOChannels,
                         num_final_other_outputs
                         , &model_2_lab2_P.HILInitialize_AOFinal
                         , NULL
                         , (t_boolean *) &model_2_lab2_P.HILInitialize_DOFinal
                         , model_2_lab2_P.HILInitialize_OOFinal
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(model_2_lab2_DW.HILInitialize_Card,
            &model_2_lab2_P.HILInitialize_AOChannels, num_final_analog_outputs,
            &model_2_lab2_P.HILInitialize_AOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_digital_outputs > 0) {
          local_result = hil_write_digital(model_2_lab2_DW.HILInitialize_Card,
            &model_2_lab2_P.HILInitialize_DOChannels, num_final_digital_outputs,
            (t_boolean *) &model_2_lab2_P.HILInitialize_DOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_other_outputs > 0) {
          local_result = hil_write_other(model_2_lab2_DW.HILInitialize_Card,
            model_2_lab2_P.HILInitialize_OOChannels, num_final_other_outputs,
            model_2_lab2_P.HILInitialize_OOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(model_2_lab2_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(model_2_lab2_DW.HILInitialize_Card);
    hil_monitor_delete_all(model_2_lab2_DW.HILInitialize_Card);
    hil_close(model_2_lab2_DW.HILInitialize_Card);
    model_2_lab2_DW.HILInitialize_Card = NULL;
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
  model_2_lab2_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  model_2_lab2_update();
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
  model_2_lab2_initialize();
}

void MdlTerminate(void)
{
  model_2_lab2_terminate();
}

/* Registration function */
RT_MODEL_model_2_lab2_T *model_2_lab2(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)model_2_lab2_M, 0,
                sizeof(RT_MODEL_model_2_lab2_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&model_2_lab2_M->solverInfo,
                          &model_2_lab2_M->Timing.simTimeStep);
    rtsiSetTPtr(&model_2_lab2_M->solverInfo, &rtmGetTPtr(model_2_lab2_M));
    rtsiSetStepSizePtr(&model_2_lab2_M->solverInfo,
                       &model_2_lab2_M->Timing.stepSize0);
    rtsiSetdXPtr(&model_2_lab2_M->solverInfo, &model_2_lab2_M->derivs);
    rtsiSetContStatesPtr(&model_2_lab2_M->solverInfo, (real_T **)
                         &model_2_lab2_M->contStates);
    rtsiSetNumContStatesPtr(&model_2_lab2_M->solverInfo,
      &model_2_lab2_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&model_2_lab2_M->solverInfo,
      &model_2_lab2_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&model_2_lab2_M->solverInfo,
      &model_2_lab2_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&model_2_lab2_M->solverInfo,
      &model_2_lab2_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&model_2_lab2_M->solverInfo, (&rtmGetErrorStatus
      (model_2_lab2_M)));
    rtsiSetRTModelPtr(&model_2_lab2_M->solverInfo, model_2_lab2_M);
  }

  rtsiSetSimTimeStep(&model_2_lab2_M->solverInfo, MAJOR_TIME_STEP);
  model_2_lab2_M->intgData.f[0] = model_2_lab2_M->odeF[0];
  model_2_lab2_M->contStates = ((real_T *) &model_2_lab2_X);
  rtsiSetSolverData(&model_2_lab2_M->solverInfo, (void *)
                    &model_2_lab2_M->intgData);
  rtsiSetSolverName(&model_2_lab2_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = model_2_lab2_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    model_2_lab2_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    model_2_lab2_M->Timing.sampleTimes =
      (&model_2_lab2_M->Timing.sampleTimesArray[0]);
    model_2_lab2_M->Timing.offsetTimes =
      (&model_2_lab2_M->Timing.offsetTimesArray[0]);

    /* task periods */
    model_2_lab2_M->Timing.sampleTimes[0] = (0.0);
    model_2_lab2_M->Timing.sampleTimes[1] = (0.001);

    /* task offsets */
    model_2_lab2_M->Timing.offsetTimes[0] = (0.0);
    model_2_lab2_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(model_2_lab2_M, &model_2_lab2_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = model_2_lab2_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    model_2_lab2_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(model_2_lab2_M, 60.0);
  model_2_lab2_M->Timing.stepSize0 = 0.001;
  model_2_lab2_M->Timing.stepSize1 = 0.001;

  /* External mode info */
  model_2_lab2_M->Sizes.checksums[0] = (1194668693U);
  model_2_lab2_M->Sizes.checksums[1] = (3479369591U);
  model_2_lab2_M->Sizes.checksums[2] = (1381867182U);
  model_2_lab2_M->Sizes.checksums[3] = (1754596142U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    model_2_lab2_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(model_2_lab2_M->extModeInfo,
      &model_2_lab2_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(model_2_lab2_M->extModeInfo,
                        model_2_lab2_M->Sizes.checksums);
    rteiSetTPtr(model_2_lab2_M->extModeInfo, rtmGetTPtr(model_2_lab2_M));
  }

  model_2_lab2_M->solverInfoPtr = (&model_2_lab2_M->solverInfo);
  model_2_lab2_M->Timing.stepSize = (0.001);
  rtsiSetFixedStepSize(&model_2_lab2_M->solverInfo, 0.001);
  rtsiSetSolverMode(&model_2_lab2_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  model_2_lab2_M->blockIO = ((void *) &model_2_lab2_B);

  {
    model_2_lab2_B.y_m = 0.0;
    model_2_lab2_B.r = 0.0;
    model_2_lab2_B.y_s = 0.0;
    model_2_lab2_B.TransportDelay = 0.0;
    model_2_lab2_B.e = 0.0;
    model_2_lab2_B.u = 0.0;
    model_2_lab2_B.Internal = 0.0;
  }

  /* parameters */
  model_2_lab2_M->defaultParam = ((real_T *)&model_2_lab2_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &model_2_lab2_X;
    model_2_lab2_M->contStates = (x);
    (void) memset((void *)&model_2_lab2_X, 0,
                  sizeof(X_model_2_lab2_T));
  }

  /* states (dwork) */
  model_2_lab2_M->dwork = ((void *) &model_2_lab2_DW);
  (void) memset((void *)&model_2_lab2_DW, 0,
                sizeof(DW_model_2_lab2_T));
  model_2_lab2_DW.HILInitialize_FilterFrequency[0] = 0.0;
  model_2_lab2_DW.HILInitialize_FilterFrequency[1] = 0.0;
  model_2_lab2_DW.TransportDelay_RWORK.modelTStart = 0.0;

  {
    int32_T i;
    for (i = 0; i < 2048; i++) {
      model_2_lab2_DW.TransportDelay_RWORK.TUbufferArea[i] = 0.0;
    }
  }

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    model_2_lab2_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  model_2_lab2_M->Sizes.numContStates = (1);/* Number of continuous states */
  model_2_lab2_M->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  model_2_lab2_M->Sizes.numY = (0);    /* Number of model outputs */
  model_2_lab2_M->Sizes.numU = (0);    /* Number of model inputs */
  model_2_lab2_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  model_2_lab2_M->Sizes.numSampTimes = (2);/* Number of sample times */
  model_2_lab2_M->Sizes.numBlocks = (10);/* Number of blocks */
  model_2_lab2_M->Sizes.numBlockIO = (7);/* Number of block outputs */
  model_2_lab2_M->Sizes.numBlockPrms = (86);/* Sum of parameter "widths" */
  return model_2_lab2_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
