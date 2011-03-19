#include <intrin.h>

template<typename T>
class mpmc_bounded_queue
{
public:
    mpmc_bounded_queue(size_t buffer_size)
        : buffer_(new cell_t [buffer_size])
        , buffer_mask_(buffer_size - 1)
    {
        typedef char assert_nothrow [__has_nothrow_assign(T) || __has_trivial_assign(T) || !__is_class(T) ? 1 : -1];
        assert((buffer_size >= 2) && ((buffer_size & (buffer_size - 1)) == 0));
        for (size_t i = 0; i != buffer_size; i += 1)
            buffer_[i].sequence_.store(i, std::memory_order_relaxed);
        enqueue_pos_.store(0, std::memory_order_relaxed);
        dequeue_pos_.store(0, std::memory_order_relaxed);
    }

    ~mpmc_bounded_queue()
    {
        delete [] buffer_;
    }

    bool enqueue(T const& data)
    {
        cell_t* cell;
        // ��������� ������� ������� ��� ���������� � �������
        size_t pos = enqueue_pos_.load(std::memory_order_relaxed);
        for (;;)
        {
            // ������� ������� �������
            cell = &buffer_[pos & buffer_mask_];
            // ��������� ������ (sequence) �������� ��������
            size_t seq = cell->sequence_.load(std::memory_order_acquire);
            intptr_t dif = (intptr_t)seq - (intptr_t)pos;
            // ������� ����� ��� ������
            if (dif == 0)
            {
                // �������� �������� ������� ��� ����������
                if (enqueue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                    break;
                // ���� �� ����������, �� �������� �������
            }
            // ������� ��� �� ����� ��� ������ (������� ����� ��� ���� ����)
            else if (dif < 0)
                return false;
            // ��� ���-�� ��������
            // ������������� ������� ������� � �������� �������
            else /* if (dif > 0) */
                pos = enqueue_pos_.load(std::memory_order_relaxed);
        }

        // � ������ ����� �� ��������������� ������� ��� ������

        // ����� ������
        cell->data_ = data;
        // �������� ������� ��� ������� ��� �����������
        cell->sequence_.store(pos + 1, std::memory_order_release);

        return true;
    }

    bool dequeue(T& data)
    {
        cell_t* cell;
        // ��������� ������� ������� ��� ���������� �� �������
        size_t pos = dequeue_pos_.load(std::memory_order_relaxed);
        for (;;)
        {
            // ������� ������� �������
            cell = &buffer_[pos & buffer_mask_];
            // ��������� ������ (sequence) �������� ��������
            size_t seq = cell->sequence_.load(std::memory_order_acquire);
            intptr_t dif = (intptr_t)seq - (intptr_t)(pos + 1);
            // ������� ����� ��� ����������
            if (dif == 0)
            {
                // �������� �������� ������� ��� ����������
                if (dequeue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                    break;
                // ���� �� ����������, �� �������� �������
            }
            // ������� ��� �� ����� ��� ����������� (������� ����� ��� ���� ����)
            else if (dif < 0)
                return false;
            // ��� ���-�� ��������
            // ������������� ������� ������� � �������� �������
            else /* if (dif > 0) */
                pos = dequeue_pos_.load(std::memory_order_relaxed);
        }

        // � ������ ����� �� ��������������� ������� ��� ������

        // ������ ������
        data = cell->data_;
        // �������� ������� ��� ������� ��� ��������� ������
        cell->sequence_.store(pos + buffer_mask_ + 1, std::memory_order_release);

        return true;
    }

private:
    struct cell_t
    {
        std::atomic<size_t>     sequence_;
        T                       data_;
    };

    static size_t const         cacheline_size = 64;
    typedef char                cacheline_pad_t [cacheline_size];

    cacheline_pad_t             pad0_;
    cell_t* const               buffer_;
    size_t const                buffer_mask_;
    cacheline_pad_t             pad1_;
    std::atomic<size_t>         enqueue_pos_;
    cacheline_pad_t             pad2_;
    std::atomic<size_t>         dequeue_pos_;
    cacheline_pad_t             pad3_;

    mpmc_bounded_queue(mpmc_bounded_queue const&);
    void operator = (mpmc_bounded_queue const&);
};



// � ��� ���������� ������������ C++0x std::atomic, ������������ ��� ���������� ������� (MSVC, x86-32):

enum memory_order
{
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst,
};

class atomic_uint
{
public:
    unsigned load(memory_order mo) const volatile
    {
        (void)mo;
        //assert(mo == memory_order_relaxed
        //    || mo == memory_order_consume
        //    || mo == memory_order_acquire
        //    || mo == memory_order_seq_cst);
        unsigned v = val_;
        _ReadWriteBarrier();
        return v;
    }

    void store(unsigned v, memory_order mo) volatile
    {
        //assert(mo == memory_order_relaxed
        //    || mo == memory_order_release
        //    || mo == memory_order_seq_cst);

        if (mo == memory_order_seq_cst)
        {
            _InterlockedExchange((long volatile*)&val_, (long)v);
        }
        else
        {
            val_ = v;
            _ReadWriteBarrier();
        }
    }

    bool compare_exchange_weak(unsigned& cmp, unsigned xchg, memory_order mo) volatile
    {
        unsigned prev = (unsigned)_InterlockedCompareExchange((long volatile*)&val_, (long)xchg, (long)cmp);
        if (prev == cmp)
            return true;
        cmp = prev;
        return false;
    }

private:
    unsigned volatile           val_;
};

template<typename T>
class atomic;

template<>
class atomic<unsigned> : public atomic_uint
{
};

int main(){
	return 0;
}