#ifndef CUTEHMI_LIBBASE_SRC_BASE_PROJECTMODEL_HPP
#define CUTEHMI_LIBBASE_SRC_BASE_PROJECTMODEL_HPP

#include "../platform.hpp"
#include "../utils/NonCopyable.hpp"

#include <QAbstractItemModel>
#include <QQmlComponent>
#include <QQmlContext>

#include <iterator>
#include <memory>

class QMenu;

namespace base {

/**
 * Project model.
 *
 * @principles
 *
 * <principle id="base.ProjectModel-internalPoiner">
 * For all valid items in the model, QModelIndex::internalPointer() is always associated with pointer
 * to object of type @p ProjectModel::Node, so that neither @p nullptr nor a pointer to any other object
 * type can be returned.
 * </principle>
 *
 * <principle id="base.ProjectModel-determinedDestructionOrder">
 * Destruction order of exposed data members is determined. The order is as following:
 *		- child nodes (children()). Each child follows the order.
 *		- UI visitor delegate (setUIVisitorDelegate()).
 *		- visitor delegate (visitorDelegate()).
 *		- node data (data()).
 *		.
 * </principle>
 *
 * @endprinciples
 */
class CUTEHMI_BASE_API ProjectModel:
	public QAbstractItemModel
{
	template <typename NODE>
	struct Iterator;

	public:
		/**
		 * Project model node. Represents a node of tree-like structure.
		 */
		struct CUTEHMI_BASE_API Node:
			public utils::NonCopyable
		{
			friend class ProjectModel;

			/**
			 * Accept delegate.
			 */
			class CUTEHMI_BASE_API VisitorDelegate
			{
				public:
					virtual ~VisitorDelegate() = default;

					/**
					 * Visit QML context. Can be overriden to set context properties.
					 * @param context QML context.
					 */
					virtual void visit(QQmlContext & context);

					/**
					 * Visit QML component. Can be overriden to associate particular QML component with a node.
					 * @param component QML component.
					 */
					virtual void visit(QQmlComponent & component);
			};

			/**
			 * Accept delegate.
			 */
			class CUTEHMI_BASE_API GUIVisitorDelegate
			{
				public:
					/**
					 * Context menu proxy. Forms a contract between visitor and visited object.
					 */
					class CUTEHMI_BASE_API ContextMenuProxy {
						public:
							/**
							 * Constructor.
							 * @param menu reference to menu pointer. Pointer should point to @p nullptr.
							 * Caller owns pointed object if it's being set by a visitor.
							 */
							ContextMenuProxy(QMenu * & menu);

							/**
							 * Pass the menu from the caller to the visited object.
							 * @param menu menu to be passed. Caller passes ownership of the @a menu object.
							 *
							 * @internal can't use std::unique_ptr, because it bitches about incomplete type.
							 * Could use reference to std::shared_ptr, but I think it would be misuse, so let's go
							 * with raw pointers and remember about undefined behaviour if delete is called
							 * on incomplete type (gcc warns about it anyways).
							 */
							void move(QMenu * menu);

						private:
							QMenu * & m_menu;
					};

					/**
					 * Visit context menu proxy.
					 * @param menuProxy context menu proxy.
					 */
					virtual void visit(ContextMenuProxy & menuProxy);

					virtual ~GUIVisitorDelegate() = default;
			};


			/**
			 *	Node data.
			 */
			struct CUTEHMI_BASE_API Data final:
				public utils::NonCopyable
			{
				/**
				 * Constructor.
				 * @param name name of the item. It will be used to display item in the view.
				 * @param object internal data object. Can be @p nullptr.
				 */
				explicit Data(const QString & name, std::unique_ptr<QObject> object = nullptr);

				/**
				 * Move constructor. Object being constructed will take over ownership of internal @a object
				 * provided in constructor or set using resetObject() function.
				 * @param other other data.
				 */
				Data(Data && other) noexcept;

				/**
				 * Destructor.
				 */
				~Data();

				/**
				 * Move assignment. Object being assigned will take over ownership of internal @a object
				 * provided in constructor or set using resetObject() function.
				 * @param other moved object.
				 * @return self-reference to object being move-assigned.
				 */
				Data & operator =(Data && other) noexcept;

				/**
				 * Set object. Can be @p nullptr.
				 * @param object internal object.
				 */
				void setObject(std::unique_ptr<QObject> object);

				/**
				 * Get internal data object.
				 * @return internal data object or @p nullptr.
				 */
				QObject * object() const;

				/**
				 * Get name.
				 * @return name.
				 */
				QString name() const;

				/**
				 * Set name.
				 * @param name node name.
				 */
				void setName(const QString & name);

				private:
					QString m_name;
					std::unique_ptr<QObject> m_object;
			};

			typedef QList<Node *> ChildrenContainer;

			/**
			 * Access node data (const version).
			 * @return node data.
			 */
			const Data & data() const;

			/**
			 * Access node data.
			 * @return node data.
			 */
			Data & data();

			/**
			 * Get parent of this node (const version).
			 * @return parent node. For root node @p nullptr is returned.
			 */
			const Node * parent() const;

			/**
			 * Get parent of this node.
			 * @return parent node. For root node @p nullptr is returned.
			 */
			Node * parent();

			/**
			 * Set visitor delegate.
			 * @param delegate visitor delegate. Can not be @p nullptr.
			 */
			void setVisitorDelegate(std::unique_ptr<VisitorDelegate> delegate);

			/**
			 * Get visitor delegate (const version).
			 * @return visitor delegate.
			 */
			const VisitorDelegate * visitorDelegate() const;

			/**
			 * Get visitor delegate.
			 * @return visitor delegate.
			 */
			VisitorDelegate * visitorDelegate();

			/**
			 * Set UI visitor delegate.
			 * @param delegate UI visitor delegate. Can not be @p nullptr.
			 */
			void setGUIVisitorDelegate(std::unique_ptr<GUIVisitorDelegate> delegate);

			/**
			 * Get UI visitor delegate (const version).
			 * @return UI visitor delegate.
			 */
			const GUIVisitorDelegate * guiVisitorDelegate() const;

			/**
			 * Get UI visitor delegate.
			 * @return UI visitor delegate.
			 */
			GUIVisitorDelegate * guiVisitorDelegate();

			/**
			 * Add child node.
			 * @param data node data.
			 * @param leaf indicates if child node is a leaf. Leaf is a node that do not have
			 * any children.
			 * @return pointer to newly created child. Child is owned by instance of this class.
			 */
			Node * addChild(Data && data, bool leaf = true);

			/**
			 * Get child at specified index (const version).
			 * @param index child index.
			 * @return child at specified index or @p nullptr if child does not exist.
			 */
			const Node * child(int index) const;

			/**
			 * Get child at specified index.
			 * @param index child index.
			 * @return child at specified index or @p nullptr if child does not exist.
			 */
			Node * child(int index);

			/**
			 * Get child index.
			 * @param child node to get index for.
			 * @return child node index or @p -1 if child was not found.
			 */
			int childIndex(const Node * child) const;

			/**
			 * Count children.
			 * @return number of children.
			 */
			int countChildren() const;

			protected:
				/**
				 * Constructor.
				 * @param data node data.
				 * @param children children container. Setting this to @p nullptr indicates that node is a leaf.
				 */
				explicit Node(Data && data, std::unique_ptr<ChildrenContainer> children = nullptr);

				/**
				 * Set parent.
				 * @param parent parent node. Must not be @p nullptr as only root node can have @p nullptr parent.
				 */
				void setParent(Node * parent);

				/**
				 * Check whether node is a leaf. If node is a leaf it does not have capability to store children.
				 * Each node is initally a leaf to avoid unnecessary allocation of children container.
				 * @return @p true if node is a leaf, @p false otherwise.
				 *
				 * @note this function is focused on capability to store children. It may return @p true even
				 * if node does not have any child.
				 */
				bool isLeaf() const;

				/**
				 * Get children (const version). If node was a leaf it becomes a node capable to store children
				 * after calling this function.
				 * @return children container.
				 */
				const ChildrenContainer * children() const;

				/**
				 * Get children. If node was a leaf it becomes a node capable to store children after calling this
				 * function.
				 * @return children container.
				 */
				ChildrenContainer * children();

				/**
				 * Destructor. Non-virtual destructor protected to prevent blind subclassing.
				 */
				~Node();

			private:
				void allocateChildren();

				//<principle ref="base.ProjectModel-determinedDestructionOrder">
				Node * m_parent;
				Data m_data;
				std::unique_ptr<VisitorDelegate> m_visitorDelegate;
				std::unique_ptr<GUIVisitorDelegate> m_guiVisitorDelegate;
				std::unique_ptr<ChildrenContainer> m_children;	///< Children nodes. It's more safe to access this member via children() function.
				//</principle>
		};

		typedef Iterator<Node> iterator;
		typedef Iterator<const Node> const_iterator;

		explicit ProjectModel(QObject * parent = 0);

		// QAbstractItemModel read access, basic function.
		QModelIndex index(int row, int column, const QModelIndex & parent) const override;

		// QAbstractItemModel read access, basic function.
		QModelIndex parent(const QModelIndex & child) const override;

		// QAbstractItemModel read access, basic function.
		int rowCount(const QModelIndex & parent) const override;

		// QAbstractItemModel read access, basic function.
		int columnCount(const QModelIndex & parent) const override;

		// QAbstractItemModel read access, basic function.
		QVariant data(const QModelIndex & index, int role) const override;

		// QAbstractItemModel read access.
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

		// QAbstractItemModel read access.
		Qt::ItemFlags flags(const QModelIndex & index) const override;

		iterator begin();

		const_iterator begin() const;

		iterator end();

		const_iterator end() const;

		/**
		 * Get root node (const version).
		 * @return root node.
		 */
		const Node & root() const;

		/**
		 * Get root node.
		 * @return root node.
		 */
		Node & root();

	protected:
		// QAbstractItemModel
		QModelIndex createIndex(int row, int column, Node * ptr) const;	// shadow

	private:
		template <typename NODE>
		struct Iterator :
			public std::iterator<std::forward_iterator_tag, NODE, std::ptrdiff_t>
		{
			typedef std::iterator<std::forward_iterator_tag, NODE, std::ptrdiff_t> Parent;

			typedef typename Parent::value_type value_type;
			typedef typename Parent::reference reference;
			typedef typename Parent::difference_type difference_type;
			typedef typename Parent::pointer pointer;
			typedef typename Parent::iterator_category iterator_category;

			// std::forward_iterator_tag
			Iterator();

			/**
			 * Constructor.
			 * @param node node.
			 */
			Iterator(NODE * node);

			// std::forward_iterator_tag
			reference operator *() const;

			// std::forward_iterator_tag
			pointer operator ->() const;

			// std::forward_iterator_tag
			Iterator & operator ++();

			// std::forward_iterator_tag
			Iterator operator ++(int);

			// std::forward_iterator_tag
			bool operator ==(const Iterator & other) const;

			// std::forward_iterator_tag
			bool operator !=(const Iterator & other) const;

			private:
				/**
				 * Get next sibling.
				 * @param node node, sibling of which should be returned.
				 * @param parent parent of @a node.
				 * @return sibling of @a node or @p nullptr if there is no child next to @a node.
				 */
				NODE * nextSibling(NODE * node, NODE * parent) const;

				NODE * m_me;
		};

		Node m_root;
};

template <typename NODE>
ProjectModel::Iterator<NODE>::Iterator():
	m_me(nullptr)
{
}

template <typename NODE>
ProjectModel::Iterator<NODE>::Iterator(NODE * node):
	m_me(node)
{
}

template <typename NODE>
typename ProjectModel::Iterator<NODE>::reference ProjectModel::Iterator<NODE>::operator *() const
{
	Q_ASSERT(m_me != nullptr);

	return *m_me;
}

template <typename NODE>
typename ProjectModel::Iterator<NODE>::pointer ProjectModel::Iterator<NODE>::operator ->() const
{
	Q_ASSERT(m_me != nullptr);

	return m_me;
}

template <typename NODE>
ProjectModel::Iterator<NODE> & ProjectModel::Iterator<NODE>::operator ++()
{
	Q_ASSERT(m_me != nullptr);

	if (m_me->countChildren() != 0)
		m_me = *(m_me->children()->begin());
	else if (m_me->parent() != nullptr) {
		NODE * sibling;
		while ((sibling = nextSibling(m_me, m_me->parent())) == nullptr) {
			m_me = m_me->parent();
			if (m_me->parent() == nullptr) {
				// Root node reached.
				m_me = nullptr;
				return *this;
			}
		}
		m_me = sibling;
	} else
		m_me = nullptr;
	return *this;
}

template <typename NODE>
ProjectModel::Iterator<NODE> ProjectModel::Iterator<NODE>::operator ++(int)
{
	Iterator<NODE> copy(*this);
	++(*this);
	return copy;
}

template <typename NODE>
bool ProjectModel::Iterator<NODE>::operator ==(const Iterator & other) const
{
	return other.m_me == m_me;
}

template <typename NODE>
bool ProjectModel::Iterator<NODE>::operator !=(const Iterator & other) const
{
	return other.m_me != m_me;
}

template <typename NODE>
NODE * ProjectModel::Iterator<NODE>::nextSibling(NODE * node, NODE * parent) const
{
	/// @todo this may require some optimization for large trees.
	return parent->child(parent->childIndex(node) + 1);
}

}

#endif