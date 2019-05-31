#pragma once
# include <stddef.h>


namespace CSTB
{
	/// This hook is used to chain objects together into a list.
	struct ListHook
	{
		ListHook* pNext;
		ListHook* pPrev;

		ListHook() { pNext = pPrev = 0; }

		/// @{
		/// null copying
		ListHook(const ListHook& rhs) { }
		ListHook& operator = (const ListHook& rhs) { }
		/// @}
	};


	struct IntrusiveWeakList
	{
	public:
		IntrusiveWeakList() { pFirst = pLast = 0; size = 0; }

		void Add(ListHook* pHook, ListHook* pInsertPos)
		{
			size++;
			if (Empty())
			{
				pFirst = pLast = pHook;
				return;
			}

			if (pInsertPos)
			{
				if (pInsertPos->pPrev)
					pInsertPos->pPrev->pNext = pHook;
				pHook->pPrev = pInsertPos->pPrev;
				pInsertPos->pPrev = pHook;
				pHook->pNext = pInsertPos;
				if (pInsertPos == pFirst)
					pFirst = pHook;
				return;
			}
			// Insert at the end.
			pLast->pNext = pHook;
			pHook->pPrev = pLast;
			pLast = pHook;
		}

		void AddHead(ListHook* pHook) { Add(pHook, 0); }


		void Remove(ListHook* pHook)
		{
			size--;
			if (pLast == pHook)
				pLast = pHook->pPrev;

			if (pFirst == pHook)
				pFirst = pHook->pNext;

			if (pHook->pPrev)
				pHook->pPrev->pNext = pHook->pNext;

			if (pHook->pNext)
				pHook->pNext->pPrev = pHook->pPrev;

			pHook->pNext = pHook->pPrev = 0;
		}

		/// Move an element to another list.
		void Move(ListHook* pHook, IntrusiveWeakList& otherList, ListHook* pInsertPos = 0)
		{
			Remove(pHook);
			otherList.Add(pHook, pInsertPos);
		}

		/// Move an element within the list
		void Move(ListHook* pHook, ListHook* pNewPos = 0) { Move(pHook, *this, pNewPos); }


		/// Move a range of elements from the list into another list.
		/// TODO: Check behavior if otherList = *this.
		void Move(ListHook* pRangeFirst, ListHook* pRangeLast, IntrusiveWeakList& otherList, ListHook* pInsertPos)
		{
			// TODO: This is possibly not needed.
			if (pRangeFirst == pRangeLast)
			{
				Move(pRangeFirst, otherList, pInsertPos);
				return;
			}

			size = -1; // invalidate size.

			// First, remove the range from the current list.
			ListHook* pBefore = pRangeFirst->pPrev;
			ListHook* pAfter = pRangeLast->pNext;
			if (pBefore) pBefore->pNext = pAfter;
			if (pAfter) pAfter->pPrev = pBefore;
			if (pFirst == pRangeFirst) pFirst = pAfter;
			if (pLast == pRangeLast) pLast = pBefore;
			pRangeFirst->pPrev = 0;
			pRangeLast->pNext = 0;

			// Now insert it into the other list.
			otherList.Add(pRangeFirst, pRangeLast, pInsertPos);
		}

		void Swap(IntrusiveWeakList& rhs)
		{
			ListHook* t;
			t = pFirst; pFirst = rhs.pFirst; rhs.pFirst = t;
			t = pLast; pLast = rhs.pLast; rhs.pLast = t;
			int ti = size; size = rhs.size; rhs.size = ti;
		}

		//void Clear() { while( Last() ) Remove( Last() ); }
		void Clear() { pFirst = pLast = 0; size = 0; }

		ListHook*			First() const { return pFirst; }
		ListHook*			Last() const { return pLast; }
		static ListHook*	Next(ListHook* pHook) { return pHook->pNext; }
		static ListHook*	Prev(ListHook* pHook) { return pHook->pPrev; }

		bool Empty() const { return pFirst == 0; }
		int Size() const
		{
			if (size < 0)
			{
				// size is invalid due to moving ranges of elements in/out of the list. Recalculate it.
				size = 0;
				for (ListHook* p = pFirst; p; p = p->pNext)
					size++;
			}
			return size;
		}

	private:
		ListHook* pFirst;
		ListHook* pLast;
		mutable int size;


		void Add(ListHook* pRangeFirst, ListHook* pRangeLast, ListHook* pInsertPos)
		{
			size = -1; // invalidate size.
			if (Empty())
			{
				pFirst = pRangeFirst;
				pLast = pRangeLast;
				return;
			}

			ListHook* pBefore, *pAfter;
			if (pInsertPos)
			{
				pAfter = pInsertPos;
				pBefore = pAfter->pPrev;

				pAfter->pPrev = pRangeLast;
				pRangeLast->pNext = pAfter;

				if (pBefore)
				{
					pBefore->pNext = pRangeFirst;
					pRangeFirst->pPrev = pBefore;
					return;
				}

				pFirst = pRangeFirst;
				return;
			}

			// Insert range at the end.
			pLast->pNext = pRangeFirst;
			pRangeFirst->pPrev = pLast;
			pLast = pRangeLast;
		}
	};
};