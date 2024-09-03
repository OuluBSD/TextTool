#include "ToolCore.h"
#include "ProtectedCommon.h"

BEGIN_TEXTLIB_NAMESPACE


const Index<String>& GetProgramGenres() {
	thread_local static Index<String> list;
	if (list.IsEmpty()) {
		list.Add("Action");
		list.Add("Adventure");
		list.Add("Role-Playing (RPG)");
		list.Add("Strategy");
		list.Add("Simulation");
		list.Add("Sports");
		list.Add("Puzzle");
		list.Add("Platformer");
		list.Add("Racing");
		list.Add("Fighting");
		list.Add("First-Person Shooter (FPS)");
		list.Add("Multiplayer Online Battle Arena (MOBA)");
		list.Add("Survival");
		list.Add("Horror");
		list.Add("Dance/Rhythm");
		list.Add("Educational");
		list.Add("Virtual Reality (VR)");
		list.Add("Massively Multiplayer Online (MMO)");
		list.Add("Productivity ");
		list.Add("Multimedia ");
		list.Add("Communication ");
		list.Add("Education ");
		list.Add("Finance ");
		list.Add("Photography ");
		list.Add("Social Media ");
		list.Add("Navigation ");
		list.Add("Health and Fitness ");
		list.Add("Weather ");
		list.Add("Shopping ");
		list.Add("Travel ");
		list.Add("News and Information ");
		list.Add("Utilities ");
		list.Add("Lifestyle ");
		list.Add("Reference ");
		list.Add("Gaming ");
		list.Add("Virtual/Augmented Reality (VR/AR)");
	}
	return list;
}

const Vector<ContentType>& GetProgrammingApproaches() {
	thread_local static Vector<ContentType> list;
	if (list.IsEmpty()) {
		list.Add().Set("Procedural programming", "procedures", "functions", "variables");
		list.Add().Set("Object-oriented programming (OOP)", "modularity", "reusability", "flexibility");
		list.Add().Set("Functional programming", "mathematical functions", "expressions", "immutable data structures");
		list.Add().Set("Event-driven programming", "events", " or system notifications", "response");
		list.Add().Set("Declarative programming", "result values", "result expressions", "result references");
		list.Add().Set("Imperative programming", "local commands", "global commands", "expression commands");
		list.Add().Set("Logical programming", "logical statements", "find a solution", "rules of logic");
		list.Add().Set("Aspect-oriented programming (AOP)", "reusable code", "multiple objects", "separation of concerns");
		list.Add().Set("Model-driven programming", "models", "the behavior of a system", "automatically generated");
		list.Add().Set("Domain-specific languages (DSLs)", "specific domains", "tasks", "abstraction");
		list.Add().Set("Low-level programming", "hardware", "assembly language", "machine code");
		list.Add().Set("High-level programming", "abstracted concepts", "syntax", "efficient");
		list.Add().Set("Event-driven concurrency", "events", "callbacks", "asynchronous execution");
		list.Add().Set("Multi-paradigm programming", "multiple programming paradigms", "take advantage of the strengths", "each approach");
		list.Add().Set("Test-driven development (TDD)", "automated tests", "write code", "pass those tests");
		list.Add().Set("Agile programming", "collaboration", "flexibility", "incremental development");
		list.Add().Set("Model-view-controller (MVC)", "model", "view", "controller");
		list.Add().Set("Rapid application development (RAD)", "building applications", "pre-made components", "rapid prototyping");
		list.Add().Set("Mobile app development", "creating applications", "different programming languages", "frameworks");
		list.Add().Set("Web development", "web applications", "websites", "web standards");
		list.Add().Set("Artificial intelligence (AI) programming", "human intelligence", "speech recognition", "decision-making");
		list.Add().Set("Machine learning", "algorithms", "learn and improve", "explicit programming");
	}
	return list;
}



END_TEXTLIB_NAMESPACE
