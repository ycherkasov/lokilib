#pragma once

#include "Threads.h"
#include "Singleton.h"
#include <cstddef>
#include <vector>

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

namespace Loki
{
    ////////////////////////////////////////////////////////////////////////////////
    // class FixedAllocator
    // Offers services for allocating fixed-sized objects
    ////////////////////////////////////////////////////////////////////////////////

    class FixedAllocator
    {
    private:
        struct Chunk
        {
            void Init(std::size_t blockSize, unsigned char blocks);
            void* Allocate(std::size_t blockSize);
            void Deallocate(void* p, std::size_t blockSize);
            void Reset(std::size_t blockSize, unsigned char blocks);
            void Release();
            unsigned char* pData_;
            unsigned char
                firstAvailableBlock_,
                blocksAvailable_;
        };

        // Internal functions        
        void DoDeallocate(void* p);
        Chunk* VicinityFind(void* p);

        // Data 
        std::size_t blockSize_;
        unsigned char numBlocks_;
        typedef std::vector<Chunk> Chunks;
        Chunks chunks_;
        Chunk* allocChunk_;
        Chunk* deallocChunk_;
        // For ensuring proper copy semantics
        mutable const FixedAllocator* prev_;
        mutable const FixedAllocator* next_;

    public:
        // Create a FixedAllocator able to manage blocks of 'blockSize' size
        explicit FixedAllocator(std::size_t blockSize = 0);
        FixedAllocator(const FixedAllocator&);
        FixedAllocator& operator=(const FixedAllocator&);
        ~FixedAllocator();

        void Swap(FixedAllocator& rhs);

        // Allocate a memory block
        void* Allocate();
        // Deallocate a memory block previously allocated with Allocate()
        // (if that's not the case, the behavior is undefined)
        void Deallocate(void* p);
        // Returns the block size with which the FixedAllocator was initialized
        std::size_t BlockSize() const
        {
            return blockSize_;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // class SmallObjAllocator
    // Offers services for allocating small-sized objects
    ////////////////////////////////////////////////////////////////////////////////

    class SmallObjAllocator
    {
    public:
        SmallObjAllocator(
            std::size_t chunkSize,
            std::size_t maxObjectSize);

        void* Allocate(std::size_t numBytes);
        void Deallocate(void* p, std::size_t size);

    private:
        SmallObjAllocator(const SmallObjAllocator&);
        SmallObjAllocator& operator=(const SmallObjAllocator&);

        typedef std::vector<FixedAllocator> Pool;
        Pool pool_;
        FixedAllocator* pLastAlloc_;
        FixedAllocator* pLastDealloc_;
        std::size_t chunkSize_;
        std::size_t maxObjectSize_;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // class SmallObject
    // Base class for polymorphic small objects, offers fast
    //     allocations/deallocations
    ////////////////////////////////////////////////////////////////////////////////

    template
        <
        template <class> class ThreadingModel = DEFAULT_THREADING,
        std::size_t chunkSize = DEFAULT_CHUNK_SIZE,
        std::size_t maxSmallObjectSize = MAX_SMALL_OBJECT_SIZE
        >
    class SmallObject : public ThreadingModel<
        SmallObject<ThreadingModel, chunkSize, maxSmallObjectSize> >
    {
        typedef ThreadingModel< SmallObject<ThreadingModel,
        chunkSize, maxSmallObjectSize> > MyThreadingModel;

        struct MySmallObjAllocator : public SmallObjAllocator
        {
            MySmallObjAllocator()
            : SmallObjAllocator(chunkSize, maxSmallObjectSize)
            {}
        };
        // The typedef below would make things much simpler, 
        //     but MWCW won't like it
        // typedef SingletonHolder<MySmallObjAllocator/*, CreateStatic, 
        //        DefaultLifetime, ThreadingModel*/> MyAllocator;

    public:
        static void* operator new(std::size_t size)
        {
#if (MAX_SMALL_OBJECT_SIZE != 0) && (DEFAULT_CHUNK_SIZE != 0)
            typename MyThreadingModel::Lock lock;
            (void)lock; // get rid of warning

            return SingletonHolder<MySmallObjAllocator, CreateStatic,
                PhoenixSingleton>::Instance().Allocate(size);
#else
            return ::operator new(size);
#endif
        }
        static void operator delete(void* p, std::size_t size)
        {
#if (MAX_SMALL_OBJECT_SIZE != 0) && (DEFAULT_CHUNK_SIZE != 0)
            typename MyThreadingModel::Lock lock;
            (void)lock; // get rid of warning

            SingletonHolder<MySmallObjAllocator, CreateStatic,
                PhoenixSingleton>::Instance().Deallocate(p, size);
#else
            ::operator delete(p);
#endif
        }
        virtual ~SmallObject() {}
    };
} // namespace Loki
