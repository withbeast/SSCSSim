#include "macro.h"
#pragma once
class Neuron
{
protected:
    int id;
    bool source;
    NeuronType type;
    bool fired;
    int firecnt;

public:
    Neuron(int id, bool isSource, NeuronType type)
    {
        firecnt = 0;
        this->id = id;
        this->source = isSource;
        this->type = type;
    }
    /// @brief 获取神经元ID
    /// @return
    int getId() { return id; };
    /// @brief 神经元是否网络输入神经元
    /// @return
    bool isSource() { return source; };
    /// @brief 神经元是否处于激活状态
    /// @return
    real isFired() { return fired; }
    /// @brief 获取神经元激活次数
    /// @return
    int getFireCount() { return firecnt; }
    /// @brief 获取神经元类型
    /// @return 神经元类型
    NeuronType getType() { return type; }

    /// @brief 接收突触电流
    /// @param in 突触电流
    virtual void recv(real in) = 0;
    /// @brief 更新神经元
    /// @param clock 当前时间
    virtual void update(int clock) = 0;
    /// @brief 清除突触电流
    virtual void clear() = 0;
    /// @brief 重置神经元参数
    virtual void reset() = 0;
    /// @brief 获取神经元的输入电流
    /// @return
    virtual real getin() = 0;
    /// @brief 获取神经元的膜电位
    /// @return
    virtual real getvm() = 0;
};

class RecordNeuron : public Neuron
{
protected:
    int clock;
    int last_fired;

public:
    RecordNeuron(int id, bool isSource, NeuronType type) : Neuron(id, isSource, type) {}
    int getLastFired() { return last_fired; }
};

class PoissonInNeuron : public RecordNeuron
{
private:
    float rate;
    real poissonRand()
    {
        return rand() % (999 + 1) / (float)(999 + 1);
    }

public:
    PoissonInNeuron(int id) : RecordNeuron(id, true, NeuronType::POISSON)
    {
    }
    virtual void clear() {}
    virtual void reset()
    {
        rate = 0;
    }
    virtual void recv(real in)
    {
        if (in >= 0 && in <= 1)
            this->rate = in;
    }
    virtual real getin()
    {
        return rate;
    }
    virtual real getvm()
    {
        return rate;
    }
    virtual void update(int clock)
    {
        fired = poissonRand() < rate;
    }
};

class LIFNeuron : public RecordNeuron
{

    /// @brief 输入电流
    real I;
    /// @brief 神经元膜电位
    real V;
    int refrac_state;

public:
    LIFNeuron(int id, bool isSource) : RecordNeuron(id, isSource, NeuronType::LIF)
    {
        V = 0;
    }
    virtual real getvm() { return V; }
    virtual real getin() { return I; };
    virtual void reset()
    {
        V = 0;
        I = 0;
    }
    virtual void clear()
    {
        I = 0;
    }
    virtual void recv(real in)
    {
        I += in;
    }
    virtual void update(int clock)
    {
        const real R = 5.1;
        const real C = 5e-3;
        const real thresh = 0.5;
        const int refrac_period = 10;
        const real V_reset = 0;

        if (refrac_state > 0)
        {
            refrac_state--;
            V = V_reset;
            fired = false;
        }
        else
        {
            real tau = R * C;
            fired = (V > thresh);
            if (fired)
            {
                refrac_state = refrac_period;
                last_fired = clock;
            }
            V += (Config::STEP / tau) * (I * R - V) - (int)fired * thresh;
        }
    }
};

struct LIF2Consts
{
    /// @brief Resting membrane potential 静息膜电位
    real V_rest;
    /// @brief Reset membrane potential 重置膜电位
    real V_reset;
    /// @brief Membrane capacitance 膜电容
    real C_m;
    /// @brief Membrane time constant 膜时间常数
    real Tau_m;
    /// @brief Refractory period 不应期
    real Refrac_period;
    /// @brief Excitatory synaptic time constant 兴奋性突触时间常数
    real Tau_exc;
    /// @brief Inhibitory synaptic time constant 抑制性突触时间常数
    real Tau_inh;
    /// @brief Spike threshold 脉冲阈值
    real V_thresh;
    /// @brief Injected current amplitude 注入电流幅值
    real I_offset;

    /// 中间参数
    real _P22;
    real _P21exc;
    real _P21inh;
    real _P11exc;
    real _P11inh;
    
    void init()
    {
        V_reset=0;
        _P22 = std::exp(-Config::DT / Tau_m);
        _P11exc = std::exp(-Config::DT / Tau_exc);
        _P11inh = std::exp(-Config::DT / Tau_inh);
        _P21exc = Tau_m * Tau_exc / (C_m * (Tau_exc - Tau_m));
        _P21inh = Tau_m * Tau_inh / (C_m * (Tau_inh - Tau_m));
    }
};

class LIFNeuron2 : public RecordNeuron
{
public:
    LIF2Consts* Consts;

private:
    /// @brief 突触输入电流
    real I_syn_exc;
    real I_syn_inh;
    /// @brief 神经元电流
    real I_exc;
    real I_inh;
    /// @brief 神经元膜电位
    real Vm;
    /// @brief 不应期状态值
    int refrac_state;
    using Neuron::Neuron;

public:
    LIFNeuron2(int id, bool isSource) : RecordNeuron(id, isSource, NeuronType::LIF2)
    {
        Vm = 0;
        Consts=new LIF2Consts();
    }
    virtual real getvm() { return Vm; }
    virtual real getin() { return I_syn_exc + I_syn_inh; }
    virtual void reset()
    {
        Vm = 0;
    }
    virtual void clear()
    {
        I_syn_exc = 0;
        I_syn_inh = 0;
    }
    virtual void recv(real in)
    {
        if (in > 0)
        {
            I_syn_exc += in;
        }
        else
        {
            I_syn_inh += in;
        }
    }
    virtual void update(int clock)
    {
        if (refrac_state > 0)
        {
            --refrac_state;
        }
        else
        {
            Vm = Consts->_P22 * Vm + I_exc * Consts->_P21exc + I_inh * Consts->_P21inh;
            Vm += (1 - Consts->_P22) * (Consts->I_offset * Consts->Tau_m / Consts->C_m + Consts->V_rest);
            I_exc *= Consts->_P11exc;
            I_inh *= Consts->_P11inh;
            if (Vm >= Consts->V_thresh)
            {
                fired = true;
                firecnt++;
                Vm = Consts->V_reset;
                refrac_state = Consts->Refrac_period;
            }
            else
            {
                I_exc += I_syn_exc;
                I_inh += I_syn_inh;
            }
        }
    }
};
