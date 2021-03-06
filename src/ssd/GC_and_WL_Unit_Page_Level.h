#ifndef GC_AND_WL_UNIT_PAGE_LEVEL_H
#define GC_AND_WL_UNIT_PAGE_LEVEL_H

#include "GC_and_WL_Unit_Base.h"
#include "NVM_PHY_ONFI.h"

namespace SSD_Components
{
	enum class GC_Block_Selection_Policy_Type {
		GREEDY,
		RGA,						/*The randomized-greedy algorithm described in: "B. Van Houdt, A Mean Field Model 
									for a Class of Garbage Collection Algorithms in Flash - based Solid State Drives,
									SIGMETRICS, 2013" and "Stochastic Modeling of Large-Scale Solid-State Storage
									Systems: Analysis, Design Tradeoffs and Optimization, SIGMETRICS, 2013".*/
		RANDOM, RANDOM_P, RANDOM_PP,/*The RANDOM, RANDOM+, and RANDOM++ algorithms described in: "B. Van Houdt, A Mean
									Field Model  for a Class of Garbage Collection Algorithms in Flash - based Solid
									State Drives, SIGMETRICS, 2013".*/
		FIFO						/*The FIFO algortihm described in "P. Desnoyers, "Analytic  Modeling  of  SSD Write
									Performance, SYSTOR, 2012".*/
		};
	class GC_and_WL_Unit_Page_Level : public GC_and_WL_Unit_Base
	{
	public:
		GC_and_WL_Unit_Page_Level(FTL* ftl, Flash_Block_Manager_Base* BlockManager, double GCThreshold, 
			GC_Block_Selection_Policy_Type BlockSelectionPolicy,
			bool PreemptibleGCEnabled, double GCHardThreshold,
			unsigned int ChannelCount, unsigned int ChipNoPerChannel, unsigned int DieNoPerChip, unsigned int PlaneNoPerDie,
			unsigned int Block_no_per_plane, unsigned int Page_no_per_block, unsigned int SectorsPerPage);
		void Setup_triggers();

		/*This function is used for implementing preemptible GC execution. If for a flash chip the free block
		* pool becomes close to empty, then the GC requests for that flash chip should be prioritized and
		* GC should go on in non-preemptible mode.*/
		bool GC_is_in_urgent_mode(const NVM::FlashMemory::Chip*);

		void CheckGCRequired(const unsigned int BlockPoolSize, const NVM::FlashMemory::Physical_Page_Address& planeAddress);
		void CheckWLRequired(const double staticWLFactor, const NVM::FlashMemory::Physical_Page_Address planeAddress);
	private:
		GC_Block_Selection_Policy_Type blockSelectionPolicy;
		NVM_PHY_ONFI * flash_controller;
		static void handle_transaction_serviced_signal_from_PHY(NVM_Transaction_Flash* transaction);
	};
}
#endif // !GC_AND_WL_UNIT_PAGE_LEVEL_H
