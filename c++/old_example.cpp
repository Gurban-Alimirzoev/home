//Definition for singly-linked list.
  struct ListNode {
      int val;
      ListNode *next;
      ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
      ListNode(int x, ListNode *next) : val(x), next(next) {}
  };
 
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode var1 = *list1;
        ListNode var2 = *list2;
        if (var1.next == nullptr && var2.next == nullptr) 
        {
            return list1;
        }
        if (var1.next == nullptr)
        {
            return list2;
        }
        if (var2.next == nullptr)
        {
            return list1;
        }
        while (var1.next != nullptr && var2.next != nullptr)
        {
            if (var1.val > var2.val) 
            {
                PushFront(var2.val);
                PopFront(var2);
            }
            else
            {
                PushFront(var1.val);
                PopFront(var1);
            }              
        }
        if (var1.next == nullptr)
        {
            PushFront(var1.val);
        }
        if (var2.next == nullptr)
        {
            PushFront(var2.val);
        }
        while(var1.next != nullptr) 
        {
            PushFront(var1.val);
            PopFront(var1);
        }
        while(var2.next != nullptr) 
        {
            PushFront(var2.val);            
            PopFront(var2);
        }
        
        /*ListNode *result_head = new ListNode;
        while(head->next != nullptr) {
            PushFront(*result_head, head->val);   
            PopFront(*head);
        }
        return result_head;*/
        return head;
    }
    
    void PopFront(ListNode &denis)
    {
        //assert(head.next != nullptr);
        ListNode *must_be_del_node = denis.next;
        denis.next = must_be_del_node->next;  
        delete must_be_del_node;
    }
    void PushFront(int new_elem)
    {
        ListNode *var = new ListNode(new_elem, head);
        //head = new ListNode(new_elem, var);
        //delete head;
        head = var;
        
        //head->next = new ListNode(new_elem, head->next);
        
    }
    
    ListNode *head = new ListNode;
};