#pragma once
# include "IntrusiveWeakList.h"
namespace CSTB
{

	/// This hook is used to chain objects together into a tree.
	struct TreeHook : public ListHook
	{
		TreeHook*			pParent;
		IntrusiveWeakList	children;

		TreeHook() { pParent = 0; }

		/// @{
		/// null copying
		TreeHook(const TreeHook& rhs) { }
		TreeHook& operator = (const TreeHook& rhs) { }
		/// @}
	};

	/// Intrusive weak tree.
	/// TODO: Prevent copying?
	struct IntrusiveWeakTree
	{
		IntrusiveWeakList m_list;


		/// Inserts a node in the tree.
		void Add(TreeHook* pNode, TreeHook* pParent = 0, TreeHook* pPos = 0)
		{
			if (pParent) pParent->children.Add(pNode, pPos);
			else m_list.Add(pNode, pPos);
			pNode->pParent = pParent;
		}


		/// Remove a node from the tree.
		void Remove(TreeHook* pNode)
		{
			if (pNode->pParent)
				pNode->pParent->children.Remove(pNode);
			else
				m_list.Remove(pNode);

			pNode->pParent = 0;
		}

		/// Clear the tree.
		void Clear() { m_list.Clear(); }

		/// swap the elements of two trees.
		void Swap(IntrusiveWeakTree& rhs) { m_list.Swap(rhs.m_list); }

		bool Empty() const { return First() == 0; }

		int Size() const;


		TreeHook*			First() const { return (TreeHook*)m_list.First(); }
		TreeHook*			Last() const { return (TreeHook*)m_list.Last(); }
		static TreeHook*	Parent(const TreeHook* p) { return p->pParent; }
		static TreeHook*	Next(const TreeHook* p) { return (TreeHook*)p->pNext; }
		static TreeHook*	Prev(const TreeHook* p) { return (TreeHook*)p->pPrev; }
		static TreeHook*	FirstChild(const TreeHook* p) { return (TreeHook*)p->children.First(); }
		static TreeHook*	LastChild(const TreeHook* p) { return (TreeHook*)p->children.Last(); }
		static TreeHook*	LogicalNext(const TreeHook* pNode, bool skipChildren = false);
		static TreeHook*	LogicalPrev(const TreeHook* pNode);

	private:
		mutable int size;
	};




	inline TreeHook* IntrusiveWeakTree::LogicalNext(const TreeHook* pNode, bool skipChildren)
	{
		if (!skipChildren && FirstChild(pNode))
			return FirstChild(pNode);
		while (pNode)
		{
			if (Next(pNode))
				return Next(pNode);
			pNode = Parent(pNode);
		}
		return 0;
	}



	inline TreeHook* IntrusiveWeakTree::LogicalPrev(const TreeHook* pNode)
	{
		TreeHook* pPrev = Prev(pNode);
		if (pPrev)
		{
			while (TreeHook* pNext = Next(pPrev))
			{
				if (pNext == pNode)
					return pPrev;
				pPrev = pNext;
			}
		}
		return Parent(pNode);
	}



	// TODO: This can be further optimized.
	inline int IntrusiveWeakTree::Size() const
	{
		if (size < 0)
		{
			// size is invalid due to moving ranges of elements in/out of the list. Recalculate it.
			size = 0;
			for (TreeHook* p = First(); p; p = LogicalNext(p))
				size++;
		}
		return size;
	}

	/// An intrusive weak tree simply chains elements (nodes) together without owning them.
	template <typename T, size_t offset>
	class IntrusiveWeakTreeT
		{
		public:
			IntrusiveWeakTreeT() {}
			void Add(T* t, T* parent = 0, T* pos = 0) { tree.Add(ToHook(t), ToHook(parent), ToHook(pos)); }
			void Remove(T* t)							{ tree.Remove(ToHook(t)); }
			void Clear()								{ tree.Clear(); }
			void Swap(IntrusiveWeakTreeT& rhs)		{ tree.Swap(rhs.tree); }
			bool Empty() const							{ return tree.Empty(); }
			int Size() const							{ return tree.Size(); }

			T* First() const { return FromHook(tree.First()); }
			T* Last() const { return FromHook(tree.Last()); }
			static T* Parent(const T* p) { return FromHook(IntrusiveWeakTree::Parent(ToHook(p))); }
			static T* Next(const T* p) { return FromHook(IntrusiveWeakTree::Next(ToHook(p))); }
			static T* Prev(const T* p) { return FromHook(IntrusiveWeakTree::Prev(ToHook(p))); }
			static T* FirstChild(const T* p) { return FromHook(IntrusiveWeakTree::FirstChild(ToHook(p))); }
			static T* LastChild(const T* p) { return FromHook(IntrusiveWeakTree::LastChild(ToHook(p))); }
			static T* LogicalNext(const T* p, bool skipChildren = false) { return FromHook(IntrusiveWeakTree::LogicalNext(ToHook(p), skipChildren)); }
			static T* LogicalPrev(const T* p) { return FromHook(IntrusiveWeakTree::LogicalPrev(ToHook(p))); }

		protected:
			IntrusiveWeakTree tree;


			static const TreeHook* ToHook(const T* t) { return t ? (const TreeHook*)(((char*)t) + offset) : 0; }
			static       TreeHook* ToHook(T* t) { return t ? (TreeHook*)(((char*)t) + offset) : 0; }

			static const T* FromHook(const TreeHook* p) { return p ? (const T*)(((char*)p) - offset) : 0; }
			static       T* FromHook(TreeHook* p) { return p ? (T*)(((char*)p) - offset) : 0; }
	};
};