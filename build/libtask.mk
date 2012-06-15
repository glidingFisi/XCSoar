TASK_SRC_DIR = $(SRC)/Engine/Task

TASK_SOURCES = \
	$(TASK_SRC_DIR)/TaskBehaviour.cpp \
	$(TASK_SRC_DIR)/TaskManager.cpp \
	$(TASK_SRC_DIR)/AbstractTask.cpp \
	$(TASK_SRC_DIR)/FAITriangleValidator.cpp \
	$(TASK_SRC_DIR)/FAITrianglePointValidator.cpp \
	$(TASK_SRC_DIR)/Ordered/OrderedTaskBehaviour.cpp \
	$(TASK_SRC_DIR)/Ordered/OrderedTask.cpp \
	$(TASK_SRC_DIR)/Ordered/TaskAdvance.cpp \
	$(TASK_SRC_DIR)/Ordered/TaskAdvanceSmart.cpp \
	$(TASK_SRC_DIR)/Ordered/Points/IntermediatePoint.cpp \
	$(TASK_SRC_DIR)/Ordered/Points/OrderedTaskPoint.cpp \
	$(TASK_SRC_DIR)/Ordered/Points/StartPoint.cpp \
	$(TASK_SRC_DIR)/Ordered/Points/FinishPoint.cpp \
	$(TASK_SRC_DIR)/Ordered/Points/ASTPoint.cpp \
	$(TASK_SRC_DIR)/Ordered/Points/AATPoint.cpp \
	$(TASK_SRC_DIR)/Ordered/AATIsoline.cpp \
	$(TASK_SRC_DIR)/Ordered/AATIsolineSegment.cpp \
	$(TASK_SRC_DIR)/Unordered/UnorderedTask.cpp \
	$(TASK_SRC_DIR)/Unordered/UnorderedTaskPoint.cpp \
	$(TASK_SRC_DIR)/Unordered/GotoTask.cpp \
	$(TASK_SRC_DIR)/Unordered/AbortTask.cpp \
	$(TASK_SRC_DIR)/Unordered/AlternateTask.cpp \
	$(TASK_SRC_DIR)/Factory/AbstractTaskFactory.cpp \
	$(TASK_SRC_DIR)/Factory/RTTaskFactory.cpp \
	$(TASK_SRC_DIR)/Factory/FAITaskFactory.cpp \
	$(TASK_SRC_DIR)/Factory/FAITriangleTaskFactory.cpp \
	$(TASK_SRC_DIR)/Factory/FAIORTaskFactory.cpp \
	$(TASK_SRC_DIR)/Factory/FAIGoalTaskFactory.cpp \
	$(TASK_SRC_DIR)/Factory/AATTaskFactory.cpp \
	$(TASK_SRC_DIR)/Factory/MixedTaskFactory.cpp \
	$(TASK_SRC_DIR)/Factory/TouringTaskFactory.cpp \
	$(TASK_SRC_DIR)/Points/TaskPoint.cpp \
	$(TASK_SRC_DIR)/Points/SampledTaskPoint.cpp \
	$(TASK_SRC_DIR)/Points/ScoredTaskPoint.cpp \
	$(TASK_SRC_DIR)/Points/TaskLeg.cpp \
	$(TASK_SRC_DIR)/ObservationZones/ObservationZoneClient.cpp \
	$(TASK_SRC_DIR)/ObservationZones/ObservationZonePoint.cpp \
	$(TASK_SRC_DIR)/ObservationZones/CylinderZone.cpp \
	$(TASK_SRC_DIR)/ObservationZones/SectorZone.cpp \
	$(TASK_SRC_DIR)/ObservationZones/LineSectorZone.cpp \
	$(TASK_SRC_DIR)/ObservationZones/SymmetricSectorZone.cpp \
	$(TASK_SRC_DIR)/ObservationZones/KeyholeZone.cpp \
	$(TASK_SRC_DIR)/ObservationZones/AnnularSectorZone.cpp \
	$(TASK_SRC_DIR)/ObservationZones/FAISectorZone.cpp \
	$(TASK_SRC_DIR)/PathSolvers/TaskDijkstra.cpp \
	$(TASK_SRC_DIR)/PathSolvers/TaskDijkstraMin.cpp \
	$(TASK_SRC_DIR)/PathSolvers/TaskDijkstraMax.cpp \
	$(TASK_SRC_DIR)/PathSolvers/IsolineCrossingFinder.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskMacCready.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskMacCreadyTravelled.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskMacCreadyRemaining.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskMacCreadyTotal.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskBestMc.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskSolveTravelled.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskCruiseEfficiency.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskEffectiveMacCready.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskMinTarget.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskOptTarget.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskGlideRequired.cpp \
	$(TASK_SRC_DIR)/Solvers/TaskSolution.cpp \
	$(TASK_SRC_DIR)/Stats/DistanceStat.cpp \
	$(TASK_SRC_DIR)/Stats/CommonStats.cpp \
	$(TASK_SRC_DIR)/Stats/ElementStat.cpp \
	$(TASK_SRC_DIR)/Stats/TaskStats.cpp \
	$(TASK_SRC_DIR)/Stats/TaskVario.cpp \
	$(TASK_SRC_DIR)/Visitors/TaskPointVisitor.cpp

$(eval $(call link-library,libtask,TASK))
